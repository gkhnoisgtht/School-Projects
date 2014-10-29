/*--------------------------------------------------------

 1. Name / Date: Daniel J Picard Jr / 4/27/14

 2. Created and tested with Java version 1.8.0-b132 64bit

 3. To compile the file in a command window run:
 > javac AsyncJokeClient.java

 4. To run the program first run the JokeServer:
 > java JokeServer

 Then run the following:
 > java AsyncJokeClient

 Or if the JokeServer is on a different machine:
 > java AsyncJokeClient IP_ADDRESS

 5. Files needed to run the program

 AsyncJokeClient.java

 Files needed to test the program
 JokeServer.java

 5. Notes:

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
import java.net.Socket;
import java.net.SocketException;
import java.util.HashMap;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Client to connect asynchronously to joke server
 *
 * @author DJ
 */
public class AsyncJokeClient {

    // client datastore
    private static ClientDatastore data;

    /**
     * static method to shutdown the client
     */
    public static void shutdown() {
        System.exit(0);
    }

    /**
     * Java boilerplate for the main method that java executes
     *
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        try {
            data = ClientDatastore.getInstance();
            // creates a buffer for user input
            BufferedReader user = null;
            // set the server location
            String serverName = "localhost";
            // reset the server location if an argument is passed to the program
            if (args.length > 0) {
                serverName = args[0];
            }
            new AsyncHandler().start();
            // java boilerplate to read user input
            user = new BufferedReader(new InputStreamReader(System.in));
            // get user name
            System.out.print("Please type your name:");
            // read user input
            String message = user.readLine();
            // add name to session data
            data.addName(message);
            // get user input
            while (true) {
                // print out user message
                System.out.print("Press enter for a joke or proverb, type quit to exit:");
                // read user input
                message = user.readLine();
                // check to see if user wants to quit
                if (message.equalsIgnoreCase("quit")) {
                    System.out.println("Quitting... Goodbye.");
                    // break out of the while statement and end the program
                    break;
                }
                // if there is no input send the server a message
                if (message.isEmpty()) {
                    // send a message to server
                    sendMessage(serverName);
                    while (data.isWaiting()) {
                        SumLoop();
                    }
                    System.out.println(data.getMessage());
                }
            }
            // catch exceptions
        } catch (IOException ex) {
            // if the connection is connection refused then server shutdown
            if (!ex.getMessage().contains("Connection refused")) {
                // otherwise print stacktrace of exception
                Logger.getLogger(AsyncJokeClient.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }

    /**
     * Static method to send message to the server
     *
     * @param host
     *
     * @throws IOException
     */
    private static void sendMessage(String host) throws IOException {
        // create objects for future use
        Socket socket = null;
        PrintStream toServer = null;
        BufferedReader fromServer = null;

        // setup socket communication paths
        socket = new Socket(host, 4444);
        // java boilerplate to get socket streams
        fromServer = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        toServer = new PrintStream(socket.getOutputStream(), true);

        // add message to client session
        data.addMessage("Give me something");
        // send message to the server
        toServer.println(data.getData());

        // close socket and server connections
        fromServer.close();
        toServer.close();
        socket.close();
        data.startWaiting();
    }

    private static void SumLoop() {
        try {
			// add values together
            System.out.print("Please enter numbers to sum: ");
            BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
            String userinput = reader.readLine();
			// if the string is empty return
            if (userinput.isEmpty()) {
                return;
            }
			// seperate values
            String[] numbers = userinput.split(" ");
            int val = 0;
			// sum the values
            for (String tmp : numbers) {
                val = val + Integer.parseInt(tmp);
            }
			// return the values
            System.out.println("Your sum is: " + val);

        } catch (IOException ex) {
            Logger.getLogger(AsyncJokeClient.class.getName()).log(Level.SEVERE, null, ex);
        }

    }
}

class AsyncHandler extends Thread {

    private int port;
    private final ClientDatastore datastore = ClientDatastore.getInstance();

    public AsyncHandler() {
        port = 3240;
    }

    public AsyncHandler(int port) {
        this.port = port;
    }

    public void run() {
        try {
			// create a udp socket listener 
            DatagramSocket udp = new DatagramSocket(port);
			// create byte array to store input
            byte[] data = new byte[1024];
            while (true) {
				// capture the data
                DatagramPacket udpPacket = new DatagramPacket(data, data.length);
                udp.receive(udpPacket);
				// decode the captured bytes
                String message = new String(data);
				// decode data
                datastore.getDataFromInput(message);
				// stop waiting for input
                datastore.stopWaiting();
            }
        } catch (SocketException ex) {
            Logger.getLogger(AsyncHandler.class.getName()).log(Level.SEVERE, null, ex);
        } catch (IOException ex) {
            Logger.getLogger(AsyncHandler.class.getName()).log(Level.SEVERE, null, ex);
        }

    }
}

/**
 * Client datastore that stores session information
 *
 * @author DJ
 */
class ClientDatastore {

    // static instance of the class for the creation of a singleton
    private static ClientDatastore instance;

    // hashmap that stores the data for the client session
    private HashMap Session = new HashMap();
    private boolean waiting = false;

    /**
     * static method to create singleton
     *
     * @return instance instance of the class
     */
    public static ClientDatastore getInstance() {
        // if instance does not exist, create it and create variables
        if (instance == null) {
            instance = new ClientDatastore();
            instance.setVariables();
        }
        // return instance
        return instance;
    }

    /**
     * set and save variables for this class
     */
    private void setVariables() {
        // see if file exists
        File file = new File("client-backup");
        if (file.exists()) {
            // if file exists then read the client session data
            readFromDisk();
        }
    }

    /**
     * Update datastore
     *
     * @param input hashmap input of data
     */
    public void updateDatastore(HashMap input) {
        // replace all session data
        Session.putAll(input);
        // save the new session to disk
        saveToDisk();
    }

    /**
     * Add name to the client session
     *
     * @param name user name
     */
    public void addName(String name) {
        // see if the client already has a key called "Name"
        Session.put("Name", name);

        // save session data to disk
        saveToDisk();
    }

    /**
     * Add message to be sent to the server
     *
     * @param message message to be sent to the server
     */
    public void addMessage(String message) {
        // if the session contains a message, replace it.
        //Otherwise put a new message in the session
        Session.put("Message", message);
        // save the session
        saveToDisk();
    }

    /**
     * return the session in a string format
     *
     * @return Session current session
     */
    public String getData() {
        return Session.toString();
    }

    /**
     * Parse the input from the server
     *
     * @param input string from the server
     *
     * @return message message from the server
     */
    public String getDataFromInput(String input) {
        // if the input is empty return
        if (input.isEmpty()) {
            return null;
        }
        // remove bad characters
        input = input.replace("{", "");
        input = input.replace("}", "");
        // split the string to start creating the hashmap
        String[] parts = input.split(", ");
        for (String part : parts) {
            // split the substring into key/value pair
            String[] subs = part.split("=");
            // remove anomolies
            if (subs.length != 2) {
                continue;
            }
            // put the data into the session
            Session.put(subs[0], subs[1]);
        }
        // return the server message
        return Session.get("Message").toString();
    }

    /**
     * Save data to disk
     */
    private void saveToDisk() {
        ObjectOutputStream outputStream = null;
        try {
            // see if the file exists
            File backup = new File("client-backup");
            if (backup.exists()) {
                backup.delete(); // if it exists, delete the file
            }
            // get the file stream
            outputStream = new ObjectOutputStream(new FileOutputStream(backup));
            // write session data to disk
            outputStream.writeObject(Session);
            // catch exceptions
        } catch (IOException ex) {
            // print stacktrace from exception
        } finally {
            try {
                // close the stream if it did not cause exception
                outputStream.close();
            } catch (IOException ex) {
                // print stacktrace from exception
            }
        }
    }

    /**
     * Read data from disk
     */
    private void readFromDisk() {
        ObjectInputStream inputStream = null;
        try {
            // see if the file exists
            File backup = new File("client-backup");
            if (!backup.exists()) {
                return; // if the file doesn't exists return
            }
            // get the file stream
            inputStream = new ObjectInputStream(new FileInputStream(backup));
            // read in the hashmap session data from the object stream
            Session = (HashMap) inputStream.readObject();
            // catch exception
        } catch (IOException | ClassNotFoundException ex) {
            // print stacktrace from exception
        } finally {
            try {
                // close the stream if it did not cause exception
                inputStream.close();
            } catch (IOException ex) {
                // print stacktrace from exception
            }
        }
    }

	// set waiting variable
    public synchronized void startWaiting() {
        waiting = true;
    }

	// stop waiting
    public synchronized void stopWaiting() {
        waiting = false;
    }

	// check to see if waiting is set
    public synchronized boolean isWaiting() {
        return waiting;
    }

	// return message from server
    public String getMessage() {
        return (String) Session.get("Message");
    }
}
