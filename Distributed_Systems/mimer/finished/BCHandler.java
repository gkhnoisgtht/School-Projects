

/*--------------------------------------------------------

 1. Name / Date: Daniel J Picard Jr / 5/25/14

 2. Created and tested with Java version 1.7.0_51 64bit

 3. To compile the file in a command window run:
 > javac -cp "C:\Program Files\Java\jdk1.7.0_51\lib\xstream-1.2.1.jar;C:\Program Files\Java\jdk1.7.0_51\lib\xpp3_min-1.1.3.4.O.jar" BCHandler.java


 4. To run the program type:
 > This program is called from shim.bat but can be activated by calling:
 > java -Dfirstarg=arg BCHandler

 5. Files needed to run the program

 BCHandler.java
 xstream-1.2.1.jar
 xpp3_min-1.1.3.4.O.jar

 ----------------------------------------------------------*/
import com.thoughtworks.xstream.XStream;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.Properties;
import java.util.logging.Level;
import java.util.logging.Logger;

// class to define the array formation that will be shared
class myDataArray {

    // number of lines being stored
    int num_lines = 0;
    // data being stored
    String[] lines = new String[8];
}

public class BCHandler {

    private static final String XMLfileName = "C:\\temp\\mimer.output";
    private static PrintWriter toXmlOutputFile;
    private static File xmlFile;

    public static void main(String args[]) {
        try {
            // get the arguments passed in from shim.bat
            Properties p = new Properties(System.getProperties());
            String argOne = p.getProperty("firstarg");
            String servername = p.getProperty("servername");
            // set default server location
            if (servername == null || servername.isEmpty()) {
                servername = "localhost";
            }

            // set basic data
            XStream xstream = new XStream();
            myDataArray da = new myDataArray();

            // open file for reading. the file is defined from shim.bat
            BufferedReader in = new BufferedReader(new FileReader(argOne));

            // read and display data
            int i = 0;
            // place data into array while simultaneously printing and looking for the end of the file
            while (((da.lines[i++] = in.readLine()) != null)) {
                System.out.println("Data is: " + da.lines[i - 1]);
            }
            // set num_lines to the correct value
            da.num_lines = i - 1;

            // turn the class into an xml representation
            String xml = xstream.toXML(da);

            // check to see if the xml file exists and delete if it does
            xmlFile = new File(XMLfileName);
            if (xmlFile.exists() == true && xmlFile.delete() == false) {
                throw (IOException) new IOException("XML file delete failed.");
            }
            // create new file for writing
            xmlFile = new File(XMLfileName);
            if (xmlFile.createNewFile() == false) {
                throw (IOException) new IOException("XML file creation failed.");
            } else {
                // write exml data to the file
                toXmlOutputFile = new PrintWriter(new BufferedWriter(new FileWriter(XMLfileName)));
                toXmlOutputFile.println("First arg to Handler is: " + argOne + "\n");
                toXmlOutputFile.println(xml);
                toXmlOutputFile.close();
            }

            // display the xml version to the user
            System.out.println("\nXML output:");
            System.out.println(xml);
            // send the xml to the server via backchannel
            sendToBC(xml, servername);

            // catch errors
        } catch (IOException ex) {
            Logger.getLogger(BCHandler.class.getName()).log(Level.SEVERE, null, ex);
        }

    }

    /**
     * Method that was stitched into this class to handle the server communication
     * it was originally written by Prof. Elliott in his BCCLient.java file
     *
     * @param sendData   data to be sent
     * @param serverName address of the server
     */
    static void sendToBC(String sendData, String serverName) {
        Socket sock;
        BufferedReader fromServer;
        PrintStream toServer;
        String textFromServer;

        try {

            // Open our connection Back Channel on server on port 2570:
            sock = new Socket(serverName, 2570);
            // get the stream for socket communications
            toServer = new PrintStream(sock.getOutputStream());

            // Will be blocking until we get ACK from server that data sent
            fromServer = new BufferedReader(new InputStreamReader(sock.getInputStream()));
            // send the xml data
            toServer.println(sendData);
            // send the EOL command
            toServer.println("end_of_xml");
            // finish socket communication
            toServer.flush();

            // Read two or three lines of response from the server,
            // and block while synchronously waiting:
            System.out.println("Blocking on acknowledgment from Server... ");
            textFromServer = fromServer.readLine();
            // read server response
            if (textFromServer != null) {
                System.out.println(textFromServer);
            }
            // close and exit
            sock.close();
        } catch (IOException x) {
            System.out.println("Socket error.");
            x.printStackTrace();
        }
    }
}
