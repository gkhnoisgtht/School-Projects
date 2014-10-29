--------------------------------------------------------------- 
This is a code snippet that matches BCClient.java 

No attention was paid to whether or not this basic code is thread-safe.

The code is to be inserted in your MyWebServer.java program.

Refer to the JokeServer for tips on how to invoke this method
in a separate thread.

Note that you will also have to, separately, return the correct
MIME type for .xyz data, for a different step in this assignment.

---------------------------------------------------------------

class myDataArray {
  int num_lines = 0;
  String[] lines = new String[10];
}

class BCWorker extends Thread {
    private Socket sock;
    private int i;
    BCWorker (Socket s){sock = s;}
    PrintStream out = null; BufferedReader in = null;

    String[] xmlLines = new String[15];
    String[] testLines = new String[10];
    String xml;
    String temp;
    XStream xstream = new XStream();
    final String newLine = System.getProperty("line.separator");
    myDataArray da = new myDataArray();
    
    public void run(){
      System.out.println("Called BC worker.");
      try{
	in =  new BufferedReader(new InputStreamReader(sock.getInputStream()));
	out = new PrintStream(sock.getOutputStream()); // to send ack back to client
	i = 0; xml = "";
	while(true){
	  temp = in.readLine();
	  if (temp.indexOf("end_of_xml") > -1) break;
	  else xml = xml + temp + newLine; // Should use StringBuilder in 1.5
	}
	System.out.println("The XML marshaled data:");
	System.out.println(xml);
	out.println("Acknowledging Back Channel Data Receipt"); // send the ack
	out.flush(); sock.close();
	
        da = (myDataArray) xstream.fromXML(xml); // deserialize / unmarshal data
	System.out.println("Here is the restored data: ");
	for(i = 0; i < da.num_lines; i++){
	  System.out.println(da.lines[i]);
	}
      }catch (IOException ioe){
      } // end run
    }
}

class BCLooper implements Runnable {
  public static boolean adminControlSwitch = true;
  
  public void run(){ // RUNning the Admin listen loop
    System.out.println("In BC Looper thread, waiting for 2570 connections");
    
    int q_len = 6; /* Number of requests for OpSys to queue */
    int port = 2570;  // Listen here for Back Channel Connections
    Socket sock;
    
    try{
      ServerSocket servsock = new ServerSocket(port, q_len);
      while (adminControlSwitch) {
	// wait for the next ADMIN client connection:
	sock = servsock.accept();
	new BCWorker (sock).start(); 
      }
    }catch (IOException ioe) {System.out.println(ioe);}
  }
}

And in main:
        [...]
	BCLooper AL = new BCLooper(); // create a DIFFERENT thread for Back Door Channel
	Thread t = new Thread(AL);
	t.start();  // ...and start it, waiting for Back Channel input
        [...]
