# Server Client Project Template

## NOTE this is not the final product yet.


### Description

This is a Server/Client project aimed to be a helpfull for the group with networking tasks.
We are going to use Cmake and This project should maintain its Cross-Platform Compatibility.
Atleast for Windows and Linux.

In here I will provide a quick brief on how to use this project for your needs.

### Compilation Steps:

#### Use Visual Studio to compile (requires version 2017 or above)

**1.** Open with CMake the project with visual studio.

**2.** Open the Utils Project first and Compile it. This is needed as a Utils for Our Client and Server Projects.

**3.** Compile The rest of the projects.
	
#### How to run:

**1.**	After compiling you need to choose what PC will be acting as the Server.
	-	in CMD run the command: "ipconfig" in windows to extract the machines IP adress, the clients will connect to it.
	
![image](/pics/ipconfig.png "example")

**2.**	Server configurations

*	Configure the port number for the server to listen on. for example: int portNumber = 9090;
*	Configure the ThreadPool size. for example: int listenAmount = 2;
*	Configure how you want/expect to send and recieve data to/from clients. 
-	This can be done by inheriting the "SendRecieveInterface" class from the Utils lib.
	
![image](/pics/ServerSideSendRecieve.png "example")

you need to implement all those virtual functions.

*	Creating the Server Class: After the previous steps we are ready to construct the Server.

![image](/pics/ServerMain.png "example")

**3.**	Client configurations

*	Configure the port number for the client to connect on.
	- for example: int connectToPort = 9090;
	
*	Configure the IP for the client to connect to the specific machine.
	- for example: int connectToIP = "127.0.0.1";
	
*	Configure how you want/expect to send and recieve data to/from server.
	-	This can be done by inheriting the "SendRecieveInterface" class from the Utils lib.
	
![image](/pics/clientSideSendRecieve.png "example")

**you need to implement all those virtual functions.**

*	Creating the Client Class:
	-	After the previous steps we are ready to construct the Client.
			
![image](/pics/ClientMain.png "example")

And you are ready to Run.

#### Run Example:

**1.**	<em>First we connect to the server so it will start listening for clients.</em>

![image](/pics/ServerBeforeConnections.png "example")
	
**2.**	<em>After we run the server we can connect to it with our clients.</em>

![image](/pics/ClientConnect.png "example")

**3.**	<em>Server after connections:</em>

![image](/pics/ServerConnect.png "example")
	
**4.**	<em>Clients after connecting:</em>

![image](/pics/ClientConnect.png "example")
	
**5.**	<em>Clients after sending messages:</em>

![image](/pics/ThirdClientRuning.png "example")
	
**6.**	<em>Server after recieving messages:</em>

![image](/pics/ServerSideWithThreeClients.png "example")
		
In this example we had set up the ThreadPool size to be equal to 2
So only two clients were processed at once.
The others connected had to wait for others to finish.

## Improvments
	
There should be improvments made and this is not the final product yet.

**1.**	The server side should in theory execute any task.
	-	So for example it can be done by passing the task you want it to solve by class pointer.
		But this class has to be templated becuase its "Any" task. which will force the server to be tempalted aswell.
		Other ways may include sending Json files with the parameters and "forward" them
		to some "execute" function which takes any parameter.

**2.**	Can implement additional ThreadPool and ThreadSafe queue for handling "Tasks" which are being sent by the clients.
	-	In summary
		- Create a listener thread (which isn't the main thread) that handles connections.
		- Adjust the ThreadPool to handle "Tasks" and not Clients.

**3.**	Should implemet Server/Client classes as Interfaces and inherite from them.
	-	That way everyone will be able to "Construct" their own implementation of Client/Server for their own use and solve their tasks.
		
**4.**	implement the "execute" function Via std::shared_ptr<std::function<...>> in the ThreadPool.

**5.**	Usage of RAII objects i.e. smartPointers, UniqueLocks, etc.
