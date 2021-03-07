// This program allows the scanning of certain files such as txt, csv, etc to be manipulated / analyzed.

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;

public class JavaArgsNB
{
  
	public static void main(String[] args)
    {
        int i, intValue, dblvalue;

		System.out.println( "\ncommand line arguments:\n");

        // Print out each argument given.
        // If more than 3 arguments are inserted they will not do anything.
		for (i = 0; i < args.length; i++)
		{
			String s = args[ i ];
			System.out.println( "argument " + i + ": " + s );
		}

        // Use the first argument to open a file.
		File file = new File( args[ 0 ] );

        // Open a file if it exists.
		try
        {
			BufferedReader br = new BufferedReader( new FileReader( file ));
			System.out.println( "\nFile opened successfully.");
			br.close();
		}
        catch (Exception e)
        {
			e.printStackTrace();
		}

        // Print out the second argument as an integer.
		intvalue = Integer.parseInt( args[ 1 ] );
		System.out.println( "\nintvalue = " + intvalue);

        // Print out the third argument as a double.
		dblvalue = Double.parseDouble( args[ 2 ] );
		System.out.println( "\ndblvalue = " + dblvalue);
	}
}
    
