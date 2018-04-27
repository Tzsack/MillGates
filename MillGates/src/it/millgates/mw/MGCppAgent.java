package it.millgates.mw;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.UnknownHostException;

import it.unibo.ai.didattica.mulino.actions.Action;
import it.unibo.ai.didattica.mulino.client.MulinoClient;
import it.unibo.ai.didattica.mulino.domain.State;
import it.unibo.ai.didattica.mulino.domain.State.Checker;

public class MGCppAgent extends MulinoClient {

	public MGCppAgent(Checker player) throws UnknownHostException, IOException {
		super(player);
	}

	private final static String CPP_PATH = "../MillGatesAgent/Release/MillGatesAgent.exe";
	private final static int PORT = 25568;

	public static void main(String[] args) {

		// Start C++ Agent

		Runtime rt = null;
		Process pr = null;
		boolean debug = true;

		if (args.length == 1 || !args[1].equals("--debug")) { 
			try {
				rt = Runtime.getRuntime();
				pr = rt.exec(CPP_PATH +" "+ args[0]);
				debug = false;
			} catch (IOException e) {
				e.printStackTrace();
			}
		}

		// Connect to C++ Agent

		ServerSocket serverSocket = null;
		Socket socket = null;
		DataInputStream agentOutput = null;
		DataOutputStream agentInput = null;

		try {
			serverSocket = new ServerSocket(PORT);
		} catch (IOException e) {
			e.printStackTrace();
		}

		System.out.println("Waiting for C++ Agent connection");

		try {
			socket = serverSocket.accept();

			System.out.println("C++ Agent connected");

			agentOutput = new DataInputStream(socket.getInputStream());
			agentInput = new DataOutputStream(socket.getOutputStream());

			State.Checker player;
			if ("White".equalsIgnoreCase(args[0]))
				player = State.Checker.WHITE;
			else
				player = State.Checker.BLACK;

			System.out.println("Connecting to server");

			MulinoClient client = new MGCppAgent(player);

			System.out.println("Connected to server");

			ActionFactory actionFactory = new DefaultActionFactory();
			StateEncoder stateEncoder = new Pos2DPawnStateEncoder();
			byte[] inputBuffer = new byte[actionFactory.maxStringLength()];
			String actionString = "";
			String stateString = "";
			Action action;
			State currentState = null;

			if (player == State.Checker.WHITE) {

				currentState = client.read();

				while (true) {

					// read move from cpp agent
					agentOutput.read(inputBuffer, 0, actionFactory.maxStringLength());
					actionString = new String(inputBuffer, "UTF-8");

					action = actionFactory.stringToAction(actionString.trim(), currentState.getCurrentPhase());
					// write move to server
					client.write(action);
					// read result of our move from server
					currentState = client.read();
					// waiting for opponent move 
					currentState = client.read();
					// write current state string to cpp agent
					stateString = stateEncoder.encode(currentState);
					agentInput.write(stateString.getBytes("UTF-8"), 0, stateString.length());
				}
			} else {

				currentState = client.read();

				while (true) {
					// waiting opponent move
					currentState = client.read();
					// write current state string to cpp agent
					stateString = stateEncoder.encode(currentState);
					agentInput.write(stateString.getBytes("UTF-8"), 0, stateString.length());
					// read move from cpp agent
					agentOutput.read(inputBuffer, 0, actionFactory.maxStringLength());
					actionString = new String(inputBuffer, "UTF-8");

					action = actionFactory.stringToAction(actionString.trim(), currentState.getCurrentPhase());
					// write move to server
					client.write(action);
					// read result of our move from server
					currentState = client.read();
				}
			}

		}
		catch (IOException | ClassNotFoundException e) {
			e.printStackTrace();
		}
		finally {
			try {

				System.out.println("Closing sockets");

				socket.close();
				serverSocket.close();
				if (!debug) {
					pr.destroy();
				}

			}
			catch (Exception e) {
				e.printStackTrace();
			}
		}

	}


}
