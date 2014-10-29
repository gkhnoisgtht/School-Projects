/*--------------------------------------------------------

 1. Name / Date: Daniel J Picard Jr / 5/9/14

 2. Created and tested with Java version 1.8.0-b132 64bit

 3. To compile the file in a command window run:
 > javac MyWebserver.java


 4. To run the program type:
 > java MyWebserver

 5. Files needed to run the program

 MyWebserver.java

 6. Notes: to have access to the PHP functionality the complete uri must be provided on line 130

 ----------------------------------------------------------*/

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.file.Files;
import java.util.Date;
import java.util.HashMap;
import java.util.StringTokenizer;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.logging.Level;
import java.util.logging.Logger;

public class MyWebserver {

    // Use the ExecutorService to handle threads, uses a cache pool for speed
    private static final ExecutorService exec = Executors.newCachedThreadPool();

    /**
     * boilerplate method for java to find and start the execution of a program
     *
     * @param args the command line arguments
     */
    public static void main(String[] args) {

        // set the port the non admin console will connect too
        int port = 2540;

        // print message about starting sessions
        System.out.println("Starting Web Server");
        try {
            // create the server socket
            ServerSocket server = new ServerSocket(port);

            // get base dir
            String base = new File(".").getAbsolutePath();

            // check to make sure the programming is still running
            while (true) {

                // launch a thread with the first client that connects
                // this will create a socket which is passed to the session worker
                // this runnable class will then be passed to the ExecutorService
                // to run the task
                handleThread(new Session(server.accept(), base));
            }

            // catch any exception such as the creation or closing of the socket
        } catch (IOException ex) {
            // print out the details of the exception
            Logger.getLogger(MyWebserver.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    /**
     * publicly accessible handle to start a new thread
     * It has the Executor Service execute the given running task in its thread pool
     *
     * @param run Runnable class that holds the executable task
     */
    public static void handleThread(Runnable run) {
        exec.execute(run);
    }

    /**
     * publicly accessible handle to stop the server
     * This operates by flipping the singleton running
     * it then continues by stopping all of the threads held by the Executor Service
     * then finishes by shutting down the Java instance
     */
    public static void shutdown() {
        exec.shutdown();
        System.exit(0);
    }

}

/**
 * Session worker that implements the Runnable class to be executed by the
 * Executor Service
 *
 * @author DJ
 */
class Session implements Runnable {

    // global varioables for the socket, mode, datastore, and session data
    private Socket sock = null;
    private final HashMap<String, String> session = new HashMap();
    private Log log = null;
    private final String base;

    /**
     * class constructor
     *
     * @param socket socket that was opened when client connected
     */
    Session(Socket socket, String dir) {
        sock = socket;
        log = Log.getInstance();
        this.base = dir;
        /**
         * sets the php path. change the variable to your current php.exe location
         */
        session.put("php", "php.exe");
    }

    /**
     * Override Runnable method for run
     */
    @Override
    @SuppressWarnings("empty-statement")
    public void run() {
        // create client and server streams instances
        BufferedReader client = null;
        PrintStream server = null;

        try {
            // java boilerplate to get datastreams from socket
            client = new BufferedReader(new InputStreamReader(sock.getInputStream()));
            server = new PrintStream(sock.getOutputStream(), true);

            // read the string from the server
            Response re = new Response(session, server, base);
            String input = client.readLine();
            log.Log(input);
            re.parse(input);
            re.send();
            // close the socket connections

            // catch any errors
        } catch (Exception ex) {
            if (ex != null) {
                // filter illegal char messages
                if (ex.getMessage().equalsIgnoreCase("Illegal char"));
                // print out stacktrace of exceptions
                log.Log(ex.getMessage());
            }
        } finally {
            try {
                // close the connection and sockets cleanly
                client.close();
                server.close();
                sock.close();
            } catch (IOException ex) {
                log.Log(ex.getMessage());
            }
        }
    }
}

/**
 *
 * @author DJ
 */
class Response {

    private final String base;
    private Log log = null;
    private HashMap<String, String> session = new HashMap<>();
    private final String delim = "\r\n";
    private File body = null;
    private String error = "";
    private PrintStream client;

    /**
     * Class constructor
     *
     * @param session current session param
     * @param dir     base dir
     */
    Response(HashMap<String, String> session, String dir) {
        this.base = dir;
        this.session = session;
        log = Log.getInstance();
    }

    /**
     * Class constructor
     *
     * @param session current session params
     * @param server  server stream to write to server
     * @param dir     base dir
     */
    Response(HashMap<String, String> session, PrintStream server, String dir) {
        this.base = dir;
        this.session = session;
        log = Log.getInstance();
        this.client = server;
    }

    /**
     * Take the string and parse it into session data
     *
     * @param input input from remote client
     */
    public void parse(String input) throws Exception {
        String[] inputs = input.split("\\r?\\n"); // can cause exception
        StringTokenizer token = new StringTokenizer(inputs[0]);
        if (input.isEmpty()) {
            return;
        }

        // Check whether the method was GET, POST, ect
        try {
            session.put("method", token.nextToken());
        } catch (Exception e) {
            error = "400 Bad Request";
            return;
        }

        // get the uri, stripping out the variables
        try {
            String file = token.nextToken();
            if (file.contains("?")) {
                session.put("uri", file.substring(0, file.indexOf("?")));
                session.put("variables", file.substring(file.indexOf("?") + 1));
                parseHeader(session.get("variables").split("&"));
            } else {
                session.put("uri", file);
            }
        } catch (Exception e) {
            log.Log(e.getMessage());
            return;
        }

        // get the file and place it in memory
        retrieveFile();
    }

    /**
     *
     * @param inputs
     */
    private void parseHeader(String[] inputs) {
        for (String input : inputs) {
            String[] params = input.split("=");
            session.put("var-" + params[0].trim(), params[1].trim());
        }
    }

    /**
     * Gets the file and places a placeholder in memory for later retrieval
     */
    private void retrieveFile() {
        // account for / by appending index.html
        if (session.get("uri").equalsIgnoreCase("/")) {
            File tmp = new File(base, "index.htm");
            File tmp2 = new File(base, "index.html");
            if (tmp.exists()) {
                body = tmp;
            } else if (tmp2.exists()) {
                body = tmp2;
            }
        } else {
            // grab file
            body = new File(base, session.get("uri"));
        }
        // check for a 404 not found condition
        if (body.exists()) {
            error = "200 OK";
        } else {
            error = "404 Not Found";
        }
    }

    /**
     * Creates and returns the header as a string
     *
     * @return header returns the full header for the file
     *
     * @throws IOException if an exception happens let the calling function handle it
     */
    private String buildHeader() throws IOException {
        StringBuilder sb = new StringBuilder();
        sb.append("HTTP/1.1 ").append(error).append(delim);
        sb.append("Date: ").append(new Date()).append(delim);
        sb.append("Server:MyWebserver").append(delim);
        sb.append("Content-Type: ").append((body.isDirectory()) ? "text/html" : Files.probeContentType(body.toPath())).append(delim);
        sb.append("Content-Length: ").append(session.get("Content-Length")).append(delim);
        sb.append(delim);
        return sb.toString();
    }

    /**
     * Makes an html page of the directory
     *
     * @return html_dir html representation of the directory
     */
    private String formatDirectory() {
        StringBuilder sb = new StringBuilder();
        sb.append("<h1>Index of ").append(body.getName()).append("</h1>");
        // Line taken from task description
        sb.append("<a href=\"/").append(body.getParentFile().getName()).append("\">Parent Directory</a>").append("<br>");
        try {
            for (String list : body.list()) {
                File f = new File(list);
                if (f.isDirectory()) {
                    sb.append("<a href=\"").append("/").append(body.getName()).append("/").append(f.getPath()).append("\">").append(f.getName()).append("</a>").append("<br>");
                } else {
                    sb.append("<a href=\"").append("/").append(body.getName()).append("/").append(f.getName()).append("\">").append(f.getName()).append("</a>").append("<br>");
                }
            }
        } catch (Exception e) {
            log.Log(e.getMessage());
        }
        return sb.toString();
    }

    /**
     * determines if there was a fatal exception
     * if not then determine if the request was for a folder/file
     * if it was a file was it executable
     *
     * @throws IOException
     */
    void send() throws IOException {
        if (!error.equalsIgnoreCase("200 OK")) {
            client.print(buildHeader());
            String err = "<h1>" + error + "</h1>";
            client.write(err.getBytes("ASCII"));
        } else {
            if (body.isDirectory()) {
                sendDir();
            } else {
                if (body.getName().contains(".fake-cgi") || body.getName().contains(".php")) {
                    sendCGI();
                } else {
                    sendFile();
                }
            }
        }
    }

    /**
     * runs the cgi/php file and collects the output. It then sends the result to the requesting page
     *
     * @throws IOException
     */
    private void sendCGI() throws IOException {
        Runtime run = Runtime.getRuntime();
        Process proc;

        // if the file contains php try to run the php settings
        if (body.getName().contains("php")) {
            String output = session.get("php") + " \"" + body.getPath() + "\" " + session.get("variables");
            proc = run.exec(output);
        } else {
            // run the cgi settings
            proc = run.exec(body.getPath() + " " + session.get("variables"));
        }
        // setup to capture the output from the called file
        BufferedReader output = new BufferedReader(new InputStreamReader(proc.getInputStream()));
        StringBuilder sb = new StringBuilder();
        String tmp;
        // read the input until null
        while (true) {
            tmp = output.readLine();
            if (tmp == null || tmp.isEmpty()) {
                break;
            }
            sb.append(tmp);
        }

        // send the data collection
        session.put("Content-Length", String.valueOf(sb.toString().length()));
        client.print(buildHeader());
        log.Log(buildHeader());
        client.write(sb.toString().getBytes("ASCII"));
        // memory cleanup
        proc.destroy();
    }

    /**
     * Sends bytes from files
     *
     * @throws IOException
     */
    private void sendFile() throws IOException {
        session.put("Content-Length", String.valueOf(body.length()));
        client.print(buildHeader());
        log.Log(buildHeader());
        client.write(Files.readAllBytes(body.toPath()));
    }

    /**
     * Sends HTML formatted directory
     *
     * @throws IOException
     */
    private void sendDir() throws IOException {
        String dir = formatDirectory();
        session.put("Content-Length", String.valueOf(dir.length()));
        client.print(buildHeader());
        log.Log(buildHeader());
        client.write(dir.getBytes("ASCII"));
    }
}

/**
 * Class to log the headers received and sent from the webserver
 *
 * @author DJ
 */
class Log {

    private static Log instance;
    private PrintWriter logger = null;

    /**
     * Creates the log and instantiates the file to be written too
     */
    Log() {
        try {
            logger = new PrintWriter(new File("MyWebserver.log"));
        } catch (FileNotFoundException ex) {
            Logger.getLogger(Log.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    /**
     * This method is a static method to access the only instance of this class
     * thus creating a singleton class that does not need to be passed to every
     * newly created class
     *
     * @return Log instance of itself
     */
    public static Log getInstance() {
        // if instance is not set then create an instance and set the variables
        if (instance == null) {
            instance = new Log();
        }
        // otherwise return the only instance of this class
        return instance;
    }

    /**
     * Writes the input string to both system out and the log file
     *
     * @param log
     */
    public void Log(String log) {
        if (log != null) {
            System.out.println(log);
            logger.write(log + "\n");
            logger.flush();
        }
    }

    /**
     * Close method to shut down the logging system
     */
    public void close() {
        logger.close();
    }

}
