import java.io.*;
import java.net.*;
import java.util.Scanner;

class senderfn
{
public static void main(String args[]) throws Exception
{
senderfn sws = new senderfn();
sws.run();
}

public void run() throws Exception
{
Scanner sc=new Scanner(System.in);
System.out.println(“Enter no of frames to be sent:”);
int n=sc.nextInt();
Socket myskt=new Socket(“localhost”,9999);
PrintStream myprint=new PrintStream(myskt.getOutputStream());

for(int i=0;i<=n;)
{
if(i==n)
{
	myprint.println(“exit”);
	break;
}

System.out.println(“Frame no “+i+” is sent”);
myprint.println(i);
BufferedReader bf=new BufferedReader(new InputStreamReader(myskt.getInputStream()));
String ack=bf.readLine();

if(ack!=null)
{
	System.out.println(“Acknowledgement was Received from receiver”);
	i++;
	Thread.sleep(4000);
}

else
	myprint.println(i);
}}}

//RECEIVER//
import java.io.*;
import java.net.*;
class receiverfn
{
	public static void main(String args[])throws Exception
	{
		receiverfn swr = new receiverfn();
		swr.run();
	}

	public void run() throws Exception
	{
		String rand=”random message”,str=”exit”;
		ServerSocket mysocket=new ServerSocket(9999);
		Socket ss_accept=mysocket.accept();
		BufferedReader ss_bf=new BufferedReader(new InputStreamReader(ss_accept.getInputStream()));
		PrintStream myprint=new PrintStream(ss_accept.getOutputStream());
		while(rand.compareTo(str)!=0)
		{
			Thread.sleep(1000);
			rand=ss_bf.readLine();
			if(rand.compareTo(str)==0)
			{ break;}

			System.out.println(“Frame “+rand+” was received”);
			Thread.sleep(500);
			myprint.println(“Received”);
		}
		System.out.println(“ALL FRAMES  WERE RECEIVED SUCCESSFULLY”);
	}
}
