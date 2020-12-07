import java.util.Scanner;

public class App
{
    // Local Scanner object.
    private static Scanner in = new Scanner(System.in);

    public static void main(String[] args)
    {
        int input;

        // Loops until break statement is activated.
        while (true)
        {
            selectAppMessage();

            input = getInput();

            // Enter the task app menu.
            if (input == 1)
            {
                TaskApp taskApp = new TaskApp();
                taskApp.taskList();
            }

            // Enter the contact app menu.
            else if (input == 2)
            {
                ContactApp contact = new ContactApp();
                contact.contactList();
            }

            else if (input == 3)
            {
                // Quit program.
                break;
            }

            else
            {
                System.out.println("Commands 1 - 3 only. Try again.");
            }
        }
    }

    // Print the main menu.
    private static void selectAppMessage()
    {
        System.out.println("Select Your Application");
        System.out.print("-----------------------\n\n");
        System.out.println("1) task list");
        System.out.println("2) contact list");
        System.out.println("3) quit");
    }

    // Get integer input.
    private static int getInput()
    {
        System.out.print("\n> ");
        return in.nextInt();
    }
}
