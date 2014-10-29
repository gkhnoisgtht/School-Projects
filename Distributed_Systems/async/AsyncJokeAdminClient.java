/*--------------------------------------------------------

 1. Name / Date: Daniel J Picard Jr / 5/27/14

 2. Created and tested with Java version 1.7u51 64bit

 3. To compile the file in a command window run:
 > javac AsyncJokeAdminClient.java

 4. To run the program first run the JokeServer:
 > java JokeServer

 Then run the following:
 > java AsyncJokeAdminClient

 Or if the JokeServer is on a different machine:
 > java AsyncJokeAdminClient IP_ADDRESS

 5. Files needed to run the program

 AsyncJokeAdminClient.java

 5. Notes:

 ----------------------------------------------------------*/

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.net.Socket;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Async Joke Admin client to connect to the AsyncJokeServer
 *
 * @author DJ
 */
public class AsyncJokeAdminClient {

    /**
     * boilerplate method for java to find and start the execution of a program
     *
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        BufferedReader user = null;
        try {
            // set default server name
            String serverName = "localhost";
            // if there are arguments set the server name to that
            if (args.length > 0) {
                serverName = args[0];
            }
            // setup socket communication paths
            user = new BufferedReader(new InputStreamReader(System.in));
            //setting up messages
            boolean run = true;
            String message;
            // transacting with the server
            while (run) {
                System.out.print("Please type a mode to run (joke-mode, proverb-mode, maintenance-mode, or shutdown): ");
                // read message from the user
                message = user.readLine();
                // send a message to the server
                sendMessage(serverName, message);

            }
            // catch exceptions
        } catch (IOException ex) {
            // if the connection is connection refused then server shutdown
            if (!ex.getLocalizedMessage().contains("Connection reset")) {
                // otherwise print stacktrace of exception
                Logger.getLogger(AsyncJokeAdminClient.class.getName()).log(Level.SEVERE, null, ex);
            } else {
                // if the server shutdown print that message
                System.out.println("Shutdown Successful");
            }
        }
    }

    /**
     * Static method to send data to the server
     *
     * @param host  server to connect too
     * @param input message to send to server
     *
     * @throws IOException possible exception from the socket connections
     */
    private static void sendMessage(String host, String input) throws IOException {
        Socket socket = null;
        PrintStream toServer = null;
        BufferedReader fromServer = null;

        // setup socket communication paths
        socket = new Socket(host, 4445);
        // java boilerplate to get server socket streams
        fromServer = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        toServer = new PrintStream(socket.getOutputStream(), true);

        //setting up messages
        boolean mistype = false;
        // transacting with the server send the given message

        switch (input) {
            case "shutdown": // shutdown server
                toServer.println("shutdown");
                break;
            case "joke-mode": // set to joke mode
                toServer.println("joke-mode");
                break;
            case "proverb-mode": // set to proverb mode
                toServer.println("proverb-mode");
                break;
            case "maintenance-mode": // set to maintenance mode
                toServer.println("maintenance-mode");
                break;
            default: // accidental mistype
                System.out.println("Unknown Command! please try again\n");
                mistype = true;
                break;
        }

        // if not a mistype get server response and print message
        if (!mistype) {
            // server can take its own time to respond
            System.out.println("Waiting for response...");
            // read from server
            input = fromServer.readLine();
            // parse input and print appropriate user message
            switch (input) {
                case "Unknown Command": // server couldn't read the message
                    System.out.println("An unknown command was sent to the server");
                    break;
                case "Done": // system change completed successfully
                    System.out.println("System mode has changed successfully!");
                    break;
                case "shutdown": // shutdown successful
                    System.out.println("Shutting down server! Goodbye...");
                    break;
                default: // something failed from the server
                    System.out.println("Unknown message from the server");
                    break;
            }
        }

    }
}
