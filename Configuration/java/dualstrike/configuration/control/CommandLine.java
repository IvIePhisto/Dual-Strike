package dualstrike.configuration.control;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class CommandLine {

	Process p;
	public static String readCmd = "bootloadHID.exe -de test.hex";
	public static String writeCmd = "bootloadHID.exe -e test.hex";

	public static void main(String[] args) {

		System.out.println("Start");
		CommandLine cl = new CommandLine(args[0]);
		System.out.println("End");
	}

	public CommandLine(String cmdPrefix) {
		if (cmdPrefix == null)
			cmdPrefix = "";

		System.out.println("Read EEPROM");
		cmdEEPROM(cmdPrefix + readCmd);
		System.out.println("Write EEPROM");
		cmdEEPROM(cmdPrefix + writeCmd);
	}

	public void cmdEEPROM(String cmd) {

		try {
			p = Runtime.getRuntime().exec(cmd);
			BufferedReader stdIn = new BufferedReader(new InputStreamReader(p
					.getInputStream()));
			BufferedReader errIn = new BufferedReader(new InputStreamReader(p
					.getErrorStream()));

			while (isProcessRunning(p) || stdIn.ready() || errIn.ready()) {
				String s;

				if (stdIn.ready()) {
					s = stdIn.readLine();

					if (s != null)
						System.out.println(s);
				} else if (errIn.ready()) {
					s = errIn.readLine();

					if (s != null)
						System.out.println("ERR: " + s);
				}
			}

		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	private static boolean isProcessRunning(Process process) {
		try {
			process.exitValue();

			return false;
		} catch (IllegalThreadStateException e) {
			return true;
		}
	}
}
