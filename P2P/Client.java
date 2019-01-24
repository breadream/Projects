import java.net.*;
import java.io.*;
 
public class Client {
       public static void main(String[] args){
            
             if(args.length != 1){
                    System.out.println("Usage : java Client ip_address ");
                    System.exit(1);
             }
            
             Socket sock = null;
             BufferedReader br = null;
             PrintWriter pw = null;
             boolean endflag = false;
            
             try{
		System.out.println("Please choose between REGISTER or LOGIN");
                    /**
                    Log into 1818 port by the received ip ( args[0] : ip address)
                   
                     1. Create Socket to connect to server 
			Get InputStream and OutputStream from Socket 
			and transform these into Buffered and PrintWriter format 
                     ***/
                    sock = new Socket(args[0], 1818);
                    pw = new PrintWriter(new OutputStreamWriter(sock.getOutputStream()));
                    br = new BufferedReader(new InputStreamReader(sock.getInputStream()));
                   
                    /**
		     2. Create BufferedReader to read keyboard input 
                        and create ReceiverThread object to print string from a server 
                     **/                  
                    BufferedReader keyboard = new BufferedReader(new InputStreamReader(System.in));
	
                    ReceiverThread it = new ReceiverThread (sock, br);
                    it.start();
                   
                    /**
		     3. Send the string to server from keyboard input line by line until it reads DISCONNECT 
                     **/                  
                    String line = null;
                    while((line = keyboard.readLine()) != null){
				   pw.println(line);
				   pw.flush();
				   if(line.equals("DISCONNECT")){
					 endflag = true;
					 break;
				   }
			  }
                    
                    System.out.println("Closing connection ... goodbye!");
                   
             } catch(Exception ex) {
                    if(!endflag)
                           System.out.println(ex);
             } finally {
                    try {
                           if(pw != null)
                                 pw.close();
                    } catch(Exception ex) {}
                   
                    try {
                           if(sock != null)
                                 sock.close();
                    } catch(Exception ex){}
             }
       }
}
      
/**
4. Create ReceiverThread object to print the string from server 
received by arugments of BufferedReader and Socket object 
**/
class ReceiverThread extends Thread {
      
       private Socket sock = null;
       private BufferedReader br = null;
       private static String file_name;
       private static int port_num_;
       public ReceiverThread(Socket sock, BufferedReader br) {
             this.sock = sock;
             this.br = br;
       }
            
       /**
       5. Read the string from server and print 
       **/
	// if file exists, send it (check the hashmap) 
       public void run(){
             try{
                    String line = null;
                    String [] line_seg; 
		    int num ;
                    while((line = br.readLine())!=null) {
			   line_seg = line.split(",");
		           num = line_seg.length;
			if (num == 4) {
				if (line_seg[0].equals("FPUT")) {
					file_name = line_seg[1];
					port_num_ = Integer.parseInt(line_seg[3]);
					new Thread(){
						public void run(){
							try {
								ReceiverThread.fileServer();
							} catch (IOException e) {
								e.printStackTrace();
							}
						}
					}.start();
				} else if (line_seg[0].equals("FGET")) {
						Socket sk = null;
						try { 
							sk = new Socket(line_seg[2], Integer.parseInt(line_seg[3]));
						} catch (Exception e) {
							System.out.println(e);
						}	
						
						File output = new File(line_seg[1]);
						FileOutputStream fos = new FileOutputStream(output);
						BufferedInputStream bis =new BufferedInputStream(sk.getInputStream());
						BufferedOutputStream bos = new BufferedOutputStream(fos);
						byte buffer[] = new byte[1024];
						int read;
						while((read = bis.read(buffer))!=-1) {
							bos.write(buffer, 0, read);
							bos.flush();
						}
						fos.close();
						 sk.close();
						System.out.println("File downloaded");
				 }
		      } // end of if (num == 4)  
		      else { 
			   System.out.println(line);
		      }
                 } // end of while	
         } catch(Exception e) {
           	//e.printStackTrace();
	 } finally {
		    try{
			   if(br!=null) {
				 br.close();
			   }
		    }catch(Exception ex) {}
		    try {
			   if(sock!=null){
				  sock.close();
			   }
		    }catch(Exception ex){}
       }
   } // End of run();

	public static void fileServer() throws IOException {
			System.out.println("File is ready..");
			ServerSocket ss= null;
			try {	
			  ss =  new ServerSocket(port_num_);	
			} catch (IOException io) {
				System.out.println("Cannot listen");
			}
			Socket sk = null;
			try {
				while(true) {
					sk = ss.accept();
					if (sk != null) {
						File f = new File(file_name);
						if (f.exists()) {
							BufferedInputStream bis = new BufferedInputStream(new FileInputStream(file_name));
							BufferedOutputStream bos = new BufferedOutputStream(sk.getOutputStream());
							byte buffer[] = new byte[1024];
							int read;
							while((read = bis.read(buffer))!=-1) {
								bos.write(buffer, 0, read);
								bos.flush();
							}
							bis.close();
							sk.close();
							bos.close();
							//ss.close();
						}
					}	
				}	
			}
			finally {
				ss.close();
			}
	} // end of fileServer()
} 
