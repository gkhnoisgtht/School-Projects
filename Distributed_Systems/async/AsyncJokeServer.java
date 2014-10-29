/*--------------------------------------------------------

 1. Name / Date: Daniel J Picard Jr / 5/27/14

 2. Created and tested with Java version 1.7u51 64bit

 3. To compile the file in a command window run:
 > javac AsyncJokeServer.java


 4. To run the program type:
 > java AsyncJokeServer

 To test the file please run the following programs in seperate windows:
 > java AsyncJokeClient
 > java AsyncJokeClientAdmin

 Or if the AsyncJokeServer is on a different machine:
 > java AsyncJokeClient IP_ADDRESS
 > java AsyncJokeClientAdmin IP_ADDRESS

 5. Files needed to run the program

 AsyncJokeServer.java

 Files needed to test the program
 AsyncJokeClient.java
 AsyncJokeClientAdmin.java

 5. Notes: Should be thread safe and randomize jokes/proverbs. However,
 * there is the chance that changing between jokes and proverbs could cause
 * this randomization to fail.

 ----------------------------------------------------------*/

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.PrintStream;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.UUID;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author DJ
 */
public class AsyncJokeServer {

    // Use the ExecutorService to handle threads, uses a cache pool for speed
    private static final ExecutorService exec = Executors.newCachedThreadPool();

    /**
     * boilerplate method for java to find and start the execution of a program
     *
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        /**
         * Print out a message about starting the admin thread
         * then launch the admin thread in the ExecutorService
         * thread creation function
         */
        System.out.println("Starting Admin Session Thread");
        handleThread(new ServerAdminSession());

        // Instatiate the mode to ensure the server should continue running
        Mode mode = Mode.getInstance();
        // set the port the non admin console will connect too
        int port = 4444;

        // print message about starting sessions
        System.out.println("Starting Session Threads");
        try {
            // create the server socket
            ServerSocket server = new ServerSocket(port);

            // check to make sure the programming is still running
            while (mode.getRunning()) {

                // launch a thread with the first client that connects
                // this will create a socket which is passed to the session worker
                // this runnable class will then be passed to the ExecutorService
                // to run the task
                // then print a message letting the user know we connected to a client
                handleThread(new ServerSessionWorker(server.accept()));
                System.out.println("Launching new client connection....");
            }

            // catch any exception such as the creation or closing of the socket
        } catch (IOException ex) {
            // print out the details of the exception
            Logger.getLogger(AsyncJokeServer.class.getName()).log(Level.SEVERE, null, ex);
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
        Mode.getInstance().stop();
        exec.shutdown();
        System.exit(0);
    }

}

/**
 *
 * @author DJ
 */
class ServerDatastore {

    // Hashmap to store the sessions of each connected client
    private HashMap<String, HashMap> SessionKeys = null;
    // proverbs were adapted from http://www.phrases.org.uk/meanings/proverbs.html
    // these were retrieved on 4/22/14. Author is Gary Martin.
    private final String[] proverbs = {
        "A. a dog is Xname's best friend",
        "B. Xname - a person is known by the company they keep",
        "C. All work and no play makes Xname a dull person",
        "D. Xname - always remember that practice makes perfect",
        "E. Xname - never forget that there are two sides to every question"};
    // jokes were adapted from http://www.jokes4us.com/miscellaneousjokes/cleanjokes.html
    // these were retrieved on 4/22/14. Author is unknown.
    // a few also came from my wife Rachel, who has terrible humor
    private final String[] jokes = {
        "A. So Xname what did the 0 say to the 8? Nice Belt!", // from my wife
        "B. Xname - why was 6 afraid of 7? Because 7 8 9!", // from my wife
        "C. Xname - do you know what you get from a pampered cow? Spoiled Milk!",
        "D. So Xname what do you call and alligator in a vest? An Investigator!",
        "E. Xname - what do you call an elephant that doesn't matter? An Irrelephant!"};

    // instance of this class, used in the creation of a singleton
    private static ServerDatastore instance = null;

    /**
     * Private method that sets the basic data for the class
     * It first checks to see if a backup file currently exists
     * if it does it will read in the file
     * if not then it will create a new Session keystore
     */
    private void setVariables() {
        File file = new File("backup");
        if (file.exists()) {
            readFromDisk();
        }
        if (SessionKeys == null) {
            SessionKeys = new HashMap<>();
        }
    }

    /**
     * This method is a static method to access the only instance of this class
     * thus creating a singleton class that does not need to be passed to every
     * newly created class
     *
     * @return ServerDatastore instance of itself
     */
    public static ServerDatastore getInstance() {
        // if instance is not set then create an instance and set the variables
        if (instance == null) {
            instance = new ServerDatastore();
            instance.setVariables();
        }
        // otherwise return the only instance of this class
        return instance;
    }

    /**
     * returns the next Joke for the series for the the specific user session
     *
     * @param id      UUID of the current session
     * @param current number of the array where the current joke is stored
     *
     * @return Joke the next joke for the session
     */
    public String getNextJoke(String id, int current) {
        ArrayList<Integer> tmp = (ArrayList<Integer>) SessionKeys.get(id).get("Order");
        return jokes[tmp.get(current)];
    }

    /**
     * returns the next Proverb for the series for the the specific user session
     *
     * @param id      UUID of the current session
     * @param current number of the array where the current proverb is stored
     *
     * @return Proverb the next proverb for the session
     */
    public String getNextProverb(String id, int current) {
        ArrayList<Integer> tmp = (ArrayList<Integer>) SessionKeys.get(id).get("Order");
        return proverbs[tmp.get(current)];
    }

    /**
     * Create a session for the current session in the keystore
     * and return the UUID of the new session
     *
     * @return UUID
     */
    public String createSession() {
        // generates a random UUID
        String id = UUID.randomUUID().toString();
        while (SessionKeys.containsKey(id)) {
            id = UUID.randomUUID().toString();
        }
        // creates arraylist
        ArrayList<Integer> tmp = new ArrayList<>();
        // adds default order to arraylist
        tmp.add(0);
        tmp.add(1);
        tmp.add(2);
        tmp.add(3);
        tmp.add(4);
        // randomizes order
        Collections.shuffle(tmp);

        // creates a hashmap to store the session data
        HashMap userData = new HashMap();
        userData.put("Order", tmp);

        // adds the session data to the list of sessions
        SessionKeys.put(id, userData);
        return id;
    }

    /**
     * Write the keystore to disk
     */
    private void saveToDisk() {
        // create an uninitialized object output stream
        ObjectOutputStream outputStream = null;
        try {
            // see if the file currently exists
            File backup = new File("backup");
            if (backup.exists()) {
                backup.delete(); // delete the current file
            }
            // after the file has been deleted or doesn't exist get the output
            // object stream for the file
            outputStream = new ObjectOutputStream(new FileOutputStream(backup));
            // write the hashmap directly to disk
            outputStream.writeObject(SessionKeys);
            // catch any error
        } catch (IOException ex) {
            // report errors to the user with a stacktrace
            Logger.getLogger(ServerDatastore.class.getName()).log(Level.SEVERE, null, ex);
        } finally {
            try {
                // close the output stream if it did not fail
                outputStream.close();
            } catch (IOException ex) {
                // if it failed catch the error and report back to the user with a stacktrace
                Logger.getLogger(ServerDatastore.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }

    /**
     * Reads in the hashmap of the keystore into the singleton on startup
     */
    private void readFromDisk() {
        // create an objectinputstream
        ObjectInputStream inputStream = null;
        try {
            // find out if the file exists
            File backup = new File("backup");
            if (!backup.exists()) {
                return; // exit if it does not exist
            }
            // if it does get the object stream to read in
            inputStream = new ObjectInputStream(new FileInputStream(backup));
            // read in the data directly into the keystore
            SessionKeys = (HashMap<String, HashMap>) inputStream.readObject();
            // catch any errors during this process
        } catch (IOException | ClassNotFoundException ex) {
            // print out a stacktrace of the logs on a failure for the user
            Logger.getLogger(ServerDatastore.class.getName()).log(Level.SEVERE, null, ex);
        } finally {
            try {
                // attempt to close the stream after reading assuming it didn't fail
                inputStream.close();
            } catch (IOException ex) {
                // if it does fail catch that error and print out a stacktrace for the user
                Logger.getLogger(ServerDatastore.class.getName()).log(Level.SEVERE, null, ex);
            }
        }

    }

    /**
     * Updates the keystore
     *
     * @param uuid    UUID of the session to update
     * @param session Hashmap of the session information that is being updated
     */
    public void updateDatastore(String uuid, HashMap<String, String> session) {
        // get current session data
        HashMap tmp = SessionKeys.get(uuid);
        // replace old data with the given data from session
        tmp.putAll(session);
        // put the new session back into the keystore
        SessionKeys.put(uuid, tmp);
        // save to disk
        saveToDisk();
    }

    /**
     * this method will get the current data stored for the session with the
     * corresponding uuid. It will then shuffle the order of the responses and
     * place the shuffled instance back into the Session keystore, writing this
     * to disk after it is complete for persistence.
     *
     * @param uuid UUID to for the connected session
     */
    void reshuffle(String uuid) {
        // get current session data for given UUID
        HashMap tmp = SessionKeys.get(uuid);
        // get the answer array
        ArrayList<Integer> order = (ArrayList<Integer>) tmp.get("Order");
        // rearrange the answer array
        Collections.shuffle(order);
        // replace the answer array back into the session information
        tmp.put("Order", order);
        // put the session back into the keystore
        SessionKeys.put(uuid, tmp);
        // save the new keystore information
        saveToDisk();
    }
}

/**
 *
 * @author DJ
 */
class Mode {

    /**
     * Mode is set to false to indicate jokes, true for proverbs
     */
    private int Mode = 0;

    /**
     * self explanatory - true means running, false means it is not
     */
    private boolean running = true;

    /**
     * instance of the class used to create a singleton
     */
    private static Mode instance = null;

    /**
     * This method is a static method to access the only instance of this class
     * thus creating a singleton class that does not need to be passed to every
     * newly created class
     *
     * @return Mode instance
     */
    public static Mode getInstance() {
        // check to see an instance exists, if not create a new one
        if (instance == null) {
            instance = new Mode();
        }
        // return the current instance
        return instance;
    }

    /**
     * change the proverbs mode
     */
    public synchronized void toProverb() {
        Mode = 1;
    }

    /**
     * change to joke mode
     */
    public synchronized void toJoke() {
        Mode = 0;
    }

    /**
     * change to maintenance mode
     */
    public synchronized void toMaintenance() {
        Mode = -1;
    }

    /**
     * set running to true
     */
    public synchronized void running() {
        running = true;
    }

    /**
     * stop running
     */
    public synchronized void stop() {
        running = false;
    }

    /**
     * returns the current running status
     *
     * @return running
     */
    public synchronized boolean getRunning() {
        return running;
    }

    /**
     * returns the current mode of operation
     *
     * @return Mode
     */
    public synchronized int getMode() {
        return Mode;
    }
}

/**
 * Session worker that implements the Runnable class to be executed by the
 * Executor Service
 *
 * @author DJ
 */
class ServerSessionWorker implements Runnable {

    // global varioables for the socket, mode, datastore, and session data
    private Mode mode = null;
    private Socket sock = null;
    private ServerDatastore data = null;
    private final HashMap<String, String> session = new HashMap();
    private InetAddress host;

    /**
     * class constructor
     *
     * @param socket socket that was opened when client connected
     */
    ServerSessionWorker(Socket socket) {
        sock = socket;
        // get singleton instance
        mode = Mode.getInstance();
        // get singleton instance
        data = ServerDatastore.getInstance();
        host = sock.getInetAddress();
    }

    /**
     * Override Runnable method for run
     */
    @Override
    public void run() {
        // create client and server streams instances
        BufferedReader client = null;
        PrintStream server = null;

        try {
            // java boilerplate to get datastreams from socket
            client = new BufferedReader(new InputStreamReader(sock.getInputStream()));
            server = new PrintStream(sock.getOutputStream(), true);

            // read the string from the server
            String input = client.readLine();
            // return string after processing a response
            String output = serverResponce(input);
            // update the current session
            data.updateDatastore(session.get("UUID"), session);

            // close the socket connections
            client.close();
            server.close();
            sock.close();

            // wait 40 seconds
            Thread.sleep(40000);
            // get the bytes from the string
            byte[] out = output.getBytes();
            // create the UDP socket to send data
            DatagramSocket serverSocket = new DatagramSocket();
            // UDP packet setup
            DatagramPacket packet = new DatagramPacket(out, out.length, host, 3240);
            // send the data
            serverSocket.send(packet);
            // catch any errors
        } catch (IOException ex) {
            // print out stacktrace of exceptions
            Logger.getLogger(ServerAdminSessionWorker.class.getName()).log(Level.SEVERE, null, ex);
        } catch (InterruptedException ex) {
            Logger.getLogger(ServerSessionWorker.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    /**
     * Servers response after receiving data from the client
     *
     * @param input
     *
     * @return Response Server response after processing
     */
    private String serverResponce(String input) {
        // parse the input string
        getDataFromInput(input);
        // if the session is not setup then set new session
        if (session.isEmpty() || session.get("UUID") == null) {
            // set the UUID, Joke, and Proverb for the session
            session.put("UUID", data.createSession());
            session.put("Joke", String.valueOf(0));
            session.put("Proverb", String.valueOf(0));
        }
        // Get the message for the session
        String message = session.get("Message");
        // respond with new message
        switch (message) {
            // returns new message
            case "Give me something":
                session.put("Message", getNext());
                break;
            default:
                // return default message
                session.put("Message", "complete");
                break;
        }
        // return the session data
        return session.toString();
    }

    /**
     * Take the string and parse it into session data
     *
     * @param input
     */
    private void getDataFromInput(String input) {
        // if empty quit
        if (input.isEmpty()) {
            return;
        }
        // remove bad characters from the input
        input = input.replace("{", "");
        input = input.replace("}", "");
        // split up the string into hashmap parts
        String[] parts = input.split(", ");
        // place each part into the session hashmap
        for (String part : parts) {
            // split key from value
            String[] subs = part.split("=");
            // remove anomalies
            if (subs.length != 2) {
                continue;
            }
            // set the key or replace the key if it exists
            session.put(subs[0], subs[1]);
        }
    }

    /**
     * Get the next string (proverb, maintenance, or joke)
     *
     * @return
     */
    private String getNext() {
        // current count of client
        int count = 0;
        // output for the client
        String output = "";
        switch (mode.getMode()) {
            case -1: // maintenance mode
                // return maintenance message
                return "The server is temporarily unavailable -- check-back shortly.";
            case 0: // joke mode
                // get current count
                count = Integer.parseInt(session.get("Joke"));
                // if the count is less than 5, send the next joke
                if (count < 5) {
                    output = data.getNextJoke(session.get("UUID"), count);
                } else {
                    // else reset count, reshuffle, and get the next joke
                    count = 0;
                    data.reshuffle(session.get("UUID"));
                    output = data.getNextJoke(session.get("UUID"), count);
                }
                // increase count
                count += 1;
                // replace Xname with the user name
                output = output.replaceAll("Xname", (session.get("Name") == null) ? "NoName" : session.get("Name"));
                // update session data
                session.put("Joke", String.valueOf(count));
                // return output
                return output;
            case 1: // proverb mode
                count = Integer.parseInt(session.get("Proverb"));
                // if the count is less than 5, send the next proverb
                if (count < 5) {
                    output = data.getNextProverb(session.get("UUID"), count);
                } else {
                    // else reset count, reshuffle, and get the next proverb
                    count = 0;
                    data.reshuffle(session.get("UUID"));
                    output = data.getNextProverb(session.get("UUID"), count);
                }
                // increase count
                count += 1;
                // replace Xname with the user name
                output = output.replaceAll("Xname", (session.get("Name") == null) ? "NoName" : session.get("Name"));
                // update session data
                session.put("Proverb", String.valueOf(count));
                // return output
                return output;
            default:
                // print an error message in case of exception
                return "An error must have occured";
        }
    }

}

/**
 * Main server program that spawns new threads as clients connect
 *
 * @author DJ
 */
class ServerAdminSession implements Runnable {

    // set port for the admin session
    int port = 4445;
    // get instance of the Mode to set the session type
    Mode mode = Mode.getInstance();

    // Override Runnable run method
    @Override
    public void run() {
        try {
            // create server socket
            ServerSocket server = new ServerSocket(port);
            // print admin message
            System.out.println("Launching new admin client connections...");

            // if mode is running keep accepting clients
            while (mode.getRunning()) {
                // spawn new admin session handler on client connection
                AsyncJokeServer.handleThread(new ServerAdminSessionWorker(server.accept()));
            }
            // catch exceptions
        } catch (IOException ex) {

            // print stacktrace to user for exceptions
            Logger.getLogger(ServerAdminSession.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
}

/**
 *
 * @author DJ
 */
class ServerAdminSessionWorker implements Runnable {

    // global vars
    private Mode mode = null;
    private Socket sock = null;

    /**
     * class constructor
     *
     * @param socket admin socket passed to class constructor
     */
    ServerAdminSessionWorker(Socket socket) {
        sock = socket;
        // gets mode singleton
        mode = Mode.getInstance();
    }

    /**
     * Overriden Runnable method for thread execution
     */
    @Override
    public void run() {
        // print server connection
        System.out.println("Connected to Client");
        // local vars for client and server
        BufferedReader client = null;
        PrintStream server = null;

        try {
            // java boilerplate to get socket streams
            client = new BufferedReader(new InputStreamReader(sock.getInputStream()));
            server = new PrintStream(sock.getOutputStream(), true);

            // output admin client session information
            System.out.println("Admin connected...");
            // create string objects for while statement
            String input, output;
            // while globally running
            while (mode.getRunning()) {
                // read line from client
                input = client.readLine();
                // print client message
                System.out.println("Admin: " + input);
                // if client message is null try again
                if (input == null) {
                    break;
                }
                // parse response and output string
                output = serverResponce(input);
                // print and send response
                System.out.println("Admin Sending: " + output);
                server.println(output);
            }
            // catch exceptions
        } catch (IOException ex) {
            // print stacktrace of exception
            Logger.getLogger(ServerAdminSessionWorker.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    /**
     * admin server response
     *
     * @param input client message
     *
     * @return Response admin client response
     */
    private String serverResponce(String input) {
        // set default output message
        String output = "Done";

        // get server response
        switch (input) {
            case "shutdown": // shutdown server
                AsyncJokeServer.shutdown();
                output = "shutdown";
                break;
            case "joke-mode": // set server to joke mode
                mode.toJoke();
                break;
            case "proverb-mode": // set server to proverb mode
                mode.toProverb();
                break;
            case "maintenance-mode": // set server to maintenance mode
                mode.toMaintenance();
                break;
            default: // if you cannot deciver, return unknown message
                output = "Unknown Command";
                break;
        }
        // return response
        return output;
    }
}
