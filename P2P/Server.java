import java.net.*;
import java.io.*;
import java.util.*;
 
public class Server {
	private static String file_name; // store args[0] as file name 	
	private static HashSet <String> client_set; // Hashset to check duplicate id  
	private static Map<String , String> client_info; // HashMap to store a pair of (client id, password)
	private static int port_num;

	// get method for file name 
	public static String getFileName() {
		return file_name;
	}
	// get method for client set 
	public static HashSet<String> getClient_set() {
		return client_set;	
	} 
 	// get method for client information 
	public static Map<String,String> getClient_info() {
		return client_info;
	} 
	// get method for port number 
	public static int getPortNumber() {
		return port_num;
	}

	/* main function */ 
	public static void main(String[] args) {
		// format check 
		if (args.length != 1) {
			System.out.println("Error ! \n Usage : <java server filename> \n Please run the program again");
			System.exit(1);
		}
		String str; // store line by line from the input file 
		String [] acct_info;  // parse the line 
		file_name = args[0]; // store as a string 
		port_num = 1818;

		// server side input file processing 
                try{
			client_set = new HashSet <String>();
			client_info = new HashMap<String, String>();

			HashMap<String, PrintWriter> hm = new HashMap<String, PrintWriter> ();
			BufferedReader file_read = new BufferedReader(new FileReader(args[0]));

			while((str = file_read.readLine()) != null){
				acct_info = str.split(",");	
				client_set.add(acct_info[0]);
				client_info.put(acct_info[0], acct_info[1]);
			}
			file_read.close();

		        ServerSocket server = new ServerSocket(port_num);
		        System.out.println("Waiting for clients ...");
			// support multi-threaded situation 
		        while(true){
			       Socket sock = server.accept();
			       ClientThread ct = new ClientThread(sock, hm);
			       ct.start();
      		        }
		 } catch(Exception e){
			System.out.println(e);
		 }
       } // End of main function 
 
} // End of Server class 
 
class ClientThread extends Thread{
	// Error Codes 
	private static final String SUCCESS = "0x00";
	private static final String ACCESS_DENIED = "0x01";
	private static final String DUPLICATE_ID = "0x02";
	private static final String INVALID_FILE = "0x03";
	private static final String INVALID_IP_PORT = "0x04";
	private static final String INVALID_FORMAT = "0xFF";
	// Command IDs 
	private static final String REGISTER = "REGISTER";
	private static final String LOGIN = "LOGIN";
	private static final String DISCONNECT = "DISCONNECT";
	private static final String MSG = "MSG";
	private static final String CLIST = "CLIST";
	private static final String FLIST = "FLIST";
	private static final String FPUT = "FPUT";
	private static final String FGET = "FGET";
	private static final String FILE_ID = "file_ID_";
	
	

        private Socket sock;
        private String id;
        private BufferedReader br;
	private PrintWriter pw;
        private HashMap <String, PrintWriter> hm;
        private boolean initFlag = false;
	private HashSet <String> client_set;  
	private Map<String , String> client_info;  
	private static HashMap <String, String> file_list; 
	private InetAddress inetAddr;
	private static HashSet <String> file_set;
	private static HashMap <String, String> id_to_file;
	private static HashMap <String, String> file_owner;

	
	private String line;
	private String separator;
	private String[] connected_client;
	private String [] line_segment; 
	private int arg_num;
	private int line_seg_num;
	private int file_id_count;
	private String ip_addr;
	private int port;

      
       // Constructor  
       public ClientThread(Socket sock, HashMap<String, PrintWriter> hm){
             this.sock = sock;
             this.hm = hm;
	     client_set = Server.getClient_set();
	     client_info = Server.getClient_info();
	     line = null;
	     separator = "";
	     connected_client = null;
	     line_segment = null;
             try {
		    file_list = new HashMap<String, String>();
		    id_to_file = new HashMap<String, String>();
		    file_set = new HashSet<String>();
		    file_owner = new HashMap<String, String>();
		    inetAddr = sock.getInetAddress();
		    ip_addr = inetAddr.getHostAddress();
		    port = sock.getPort();
			
                    br = new BufferedReader(new InputStreamReader(sock.getInputStream()));
                    initFlag = true;
        	    pw = new PrintWriter(new OutputStreamWriter(sock.getOutputStream()));
		    System.out.println("New client is trying to access...");

	     } catch(Exception e){
	        	System.out.println(e);
	     } 
    }
	
       // Read client's input line after login 
       public void run(){
          try{
		    String first_line = null;
		    while ((first_line = br.readLine()) != null) {
			connected_client = first_line.split(",");
			arg_num = connected_client.length;
			if (connected_client[0].equals(REGISTER) && arg_num == 3) { // Check if the first keyword is REGISTER 
				if (client_set.contains(connected_client[1])) { // Duplicate id, already in file 
					System.out.println(DUPLICATE_ID);
					pw.println("Duplicate client id! Try again ");
					pw.flush();
				}
			        else { // Register success 
					System.out.println(SUCCESS);	
					client_set.add(connected_client[1]);
					client_info.put(connected_client[1], connected_client[2]);
					Writer output;
					output = new BufferedWriter(new FileWriter(Server.getFileName(), true));
					output.append(connected_client[1] + "," + connected_client[2] + "\n");
					output.close();
					id = connected_client[1];
					synchronized(hm) {
			  		     hm.put(id, pw);
			         	}
					pw.println("Welcome!");
					pw.flush();
					break;
				}
			} else if (connected_client[0].equals(LOGIN) && arg_num == 3) { // Check if the first keyword is LOGIN 
				if (!client_set.contains(connected_client[1])) { // No client id in the database 
					System.out.println(ACCESS_DENIED);
					pw.println("No sucn client_ID.. Try again");
					pw.flush();
				} else if (hm.containsKey(connected_client[1])) {
					System.out.println(ACCESS_DENIED);
					pw.println("Duplicate id exists in the active clients list.. Try again");
					pw.flush();
				} else if (client_set.contains(connected_client[1]) && connected_client[2].equals(client_info.get(connected_client[1])) && hm.containsKey(connected_client[1]) == false) {
					System.out.println(SUCCESS);
					pw.println("Welcome!");
					pw.flush();
					id = connected_client[1];
					synchronized (hm) {
					      hm.put(id, pw);
					}
					break;
				} else {
					System.out.println(ACCESS_DENIED);
					pw.println("Wrong Password! Try again ");
					pw.flush();
		    			//first_line = br.readLine();
				}
		  }
		  else {
				System.out.println(INVALID_FORMAT);
				pw.println("Invalid format! Try again ..");
				pw.flush();
		  }
              } // End of while(first_line != null)  




		/*** After login */
		while ((line = br.readLine())!= null) {
			line_segment = line.split(",");
			line_seg_num = line_segment.length;
				
			if (line_segment[0].equals(DISCONNECT) && line_seg_num == 1) { // Anytime when a client hit DISCONNECT, it closes a connection 
				break;
			}
			else if (line_segment[0].equals(CLIST) && line_seg_num == 1) { // Show active client list
				System.out.print(SUCCESS + ",");
				Set<String> keys = hm.keySet();
				for (String clist: keys) {
					System.out.print(separator + clist);
					separator = ",";
				}
				System.out.println(" ");
				separator = "";
			}
			else if (line_segment[0].equals(MSG) && line_seg_num == 2) { // Broadcast message to all other active clients 
				System.out.println(SUCCESS);
				broadcast(id ,id + " : " +  line_segment[1]);
			}
			else if (line_segment[0].equals(FLIST)) { // Show the available file list 
				System.out.print(SUCCESS + ",");
				for (Map.Entry<String, String> e : file_list.entrySet()) {
					//String [] value = e.getValue().split(",");
					System.out.print(separator + e.getValue()); 
					separator = ",";
				}
				System.out.println(" ");
				separator = "";
			}
			else if (line_segment[0].equals(FPUT) && line_seg_num == 4) { 
				int port_num_FPUT = Integer.parseInt(line_segment[3]);
				if (line_segment[2].equals(ip_addr) && port_num_FPUT <= 65536 && port_num_FPUT >=1024) { // check valid port and ip address
					if (!file_set.contains(line_segment[1]) || file_set.isEmpty()) {
						System.out.println(SUCCESS);
						String FPUT_file_name = line_segment[1];
						String file_id = FILE_ID + Integer.toString(file_id_count);
						String existing = file_list.get(id);
						String updated_value = file_id + "," + FPUT_file_name;
						synchronized(file_list) { 
							file_list.put(id, existing == null ? updated_value : existing + "," + updated_value);	
							id_to_file.put(file_id, FPUT_file_name);
							file_owner.put(file_id, id + "," + line_segment[3]);
						}
						file_set.add(FPUT_file_name);
						file_id_count++;
						pw.println(FPUT + "," + FPUT_file_name + "," +  ip_addr + ","+ line_segment[3]);
						pw.flush();
					}
					else {
						System.out.println(INVALID_FILE);
						pw.println("Invalid file! Try again...");
						pw.flush();
					}
				} else {
					System.out.println(INVALID_IP_PORT);
					pw.println("Invalid ip address or port number! Try again...");
					pw.flush();
				}
			}
			else if (line_segment[0].equals(FGET) && line_seg_num == 2) {
				if (file_set.contains(id_to_file.get(line_segment[1]))) {
					String [] str = file_owner.get(line_segment[1]).split(",");;
					String port_number = str[1]; 
					System.out.println(SUCCESS + "," + ip_addr + "," + port_number); 
					String file_name = id_to_file.get(line_segment[1]);
					pw.println(FGET+"," + file_name + ","+ ip_addr + ","+ port_number);
					pw.flush();
				}
				else {
					System.out.println(INVALID_FILE);
					pw.println("Invalid file! Try again...");
					pw.flush();
				}
			}
			else {
				System.out.println(INVALID_FORMAT);
				pw.println("Invalid format! Try again ..");
				pw.flush();
			}
		} // End of while loop 
     
	  } catch(Exception e) {
		System.out.println(e);
	  } finally {
		 synchronized(hm){
		       if (!file_list.isEmpty()) {
			       String [] file_name = file_list.get(id).split(",");
			       for (int i = 1; i < file_name.length; i+= 2) {
				       file_set.remove(file_name[i]);
			       }
			       id_to_file.remove(file_name[0]);
			       file_list.remove(id);
			}
		       hm.remove(id);
		 }
		 broadcast(id, id + " is logged out");
		 try {
		       if(sock != null)
			  sock.close();
		 } catch(Exception ex) {}
	  }  

	}
      
         public void broadcast(String id, String msg){
                    synchronized(hm) {
			   HashMap<String, PrintWriter> temp = new HashMap<String, PrintWriter>(hm);
			   temp.remove(id); // excludes sender 
                           Collection collection = temp.values(); // get the printwriters 
                           Iterator iter = collection.iterator(); // to iterate through printWriters 
                           while(iter.hasNext()){
                                 PrintWriter pw = (PrintWriter)iter.next();
                                 pw.println(msg);
                                 pw.flush();
                           }  
                    }  
	
         } // End of broadcast 
	
      
}
