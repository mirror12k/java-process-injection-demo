import java.io.*;

public class Demo {
	/* our shellcode... populate this from Metasploit */
	byte shell[] = new byte[] { (byte)0xCC };

	public native void inject(byte[] me);

	public Demo() {
		System.load(System.getProperty("user.dir") +"/injector.dll");
		System.out.println("injecting:" + shell[0]);
		inject(shell);
		System.out.println("done!");
	}

	public static void main(String args[]) {
		new Demo();
	}
}
