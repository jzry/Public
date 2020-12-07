import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

public class ContactApp
{
    // Local Scanner object.
    private static Scanner in = new Scanner(System.in);

    public static void main(String[] args)
    {
        contactList();
    }

    public static void contactList()
    {
        int input;

        // Loops until break statement is activated.
        while (true)
        {
            printMenu();

            input = getInput();

            // Enter the option menu loop.
            if (input == 1)
            {
                // Instantiate a new encapsulated contact list object.
                ContactList contacts = new ContactList();
                System.out.println("new contact list has been created\n");

                // Open option menu loop.
                contactMenu(contacts);
            }

            // Open up the file.
            else if (input == 2)
            {
                String fileName = getString("Enter the file name to load: ");
                loadFile(fileName);
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

    // Enter the option menu.
    private static void contactMenu(ContactList contacts)
    {
        int input;

        // Loops until break statement is activated.
        while (true)
        {
            printContactMenu();

            input = getInput();

            // The input actions.
            if (input == 1)
                viewContacts(contacts);
            else if (input == 2)
                addContact(contacts);
            else if (input == 3)
                editContact(contacts);
            else if (input == 4)
                removeContact(contacts);
            else if (input == 5)
                saveList(contacts);
            else if (input == 6)
                break;
            else
                System.out.println("Commands 1 through 6 only. Try again.");
        }
    }

    // --------
    // Commands
    // --------

    // View contact list.
    private static void viewContacts(ContactList contacts)
    {
        System.out.println("\nCurrent Contacts");
        System.out.println("-------------\n");
        System.out.println(contacts.output());
    }

    // Add a contacts (first, last, number, email).
    private static void addContact(ContactList contacts)
    {
        // Get data for task object.
        in.nextLine();
        String first_name = getString("First name: ");
        String last_name = getString("Last name: ");
        String number = getString("Phone number (xxx-xxx-xxxx): ");
        String email = getString("Email address (x@y.z): ");

        // Send input to be placed in new TaskItem object.
        try
        {
            ContactItem newContact = new ContactItem(first_name, last_name, number, email);
            contacts.addContact(newContact);
        }
        catch (StringIndexOutOfBoundsException e)
        {
            System.out.println("WARNING: string index out of bounds; task not created");
        }
        catch (ContactItem.InvalidDateException e)
        {
            System.out.println("WARNING: invalid due date; task not created");
        }
        catch (ContactItem.InvalidTitleException e)
        {
            System.out.println("WARNING: title must be at least 1 character long; task not created");
        }
    }

    // Display message before user input.
    private static int contactPrompt(ContactList contacts, String status, String action)
    {
        System.out.println("\n\n" + status + " Contacts\n----------\n");
        System.out.println(contacts);
        System.out.print("Which contact will you " + action + "? ");
        return getInput();
    }

    // Edit a contact (first, last, number, email).
    private static void editContact(ContactList contacts)
    {
        try
        {
            in.nextLine();
            int index = contactPrompt(contacts, "Current", "edit");
            in.nextLine();

            // Enter new information for task and send to be edited.
            String first_name = getString("Enter a new first name for contact " + index + ": ");
            String last_name = getString("Enter a new last name for contact " + index + ": ");
            String number = getString("Enter a new phone number (xxx-xxx-xxxx) for contact " + index + ": ");
            String email = getString("Enter a new email address (x@y.z) for contact " + index + ": ");
            contacts.editContactList(index, first_name, last_name, number, email);
        }
        catch (IndexOutOfBoundsException e)
        {
            System.out.println("WARNING: invalid task choice; task cannot be edited");
        }
    }

    // Remove a contact.
    private static void removeContact(ContactList contacts)
    {
        try
        {
            in.nextLine();
            int index = contactPrompt(contacts, "Current", "remove");
            contacts.removeContact(index);
        }
        catch (IndexOutOfBoundsException e)
        {
            System.out.println("WARNING: invalid task choice; contact cannot be removed");
        }
    }

    // -----------
    // File saving
    // -----------

    // Make a file and save task contents into it.
    private static void makeFile(ContactList contacts, String fileName) throws IOException
    {
        // Make a new file object.
        File file = new File(fileName);

        // Create a new file.
        if (!file.createNewFile())
        {
            System.out.println("File already exists");
        }

        // Write to the created file.
        FileWriter writer = new FileWriter(fileName);

        // Write tasks of task list into the file.
        writer.write(contacts.output());

        // Close the input stream.
        writer.close();

        System.out.println("contact list has been saved");
    }

    // Save the current list to a file.
    private static void saveList(ContactList contacts)
    {
        in.nextLine();
        String fileName = getString("Enter the filename to save as: ");

        try
        {
            // Make a new file to save the task list to.
            makeFile(contacts, fileName);
        }
        catch (IOException e)
        {
            System.out.println("An error occurred.");
        }
    }

    // --------
    // Extras
    // -------

    // Get integer input.
    private static int getInput()
    {
        System.out.print("\n> ");
        return in.nextInt();
    }

    // Get string input.
    private static String getString(String prompt)
    {
        System.out.print(prompt);
        return in.nextLine();
    }

    // Load a file into the program.
    private static void loadFile(String fileName)
    {
        in.nextLine();
        ContactList contacts = new ContactList();

        try
        {
            // Make a new file object.
            File file = new File(fileName);

            // Create a scanner to read the file.
            Scanner reader = new Scanner(file);

            int count = 0;

            while (reader.hasNextLine())
            {
                String first_name = reader.nextLine();
                String last_name = reader.nextLine();
                String number = reader.nextLine();
                String email = reader.nextLine();

                // Send them to new task list.
                ContactItem newContact = new ContactItem(first_name, last_name, number, email);
                contacts.addContact(newContact);

                // Increment count line.
                count++;
            }

            // Close the file reader.
            reader.close();

            System.out.println("Contact list has been loaded from " + fileName);

            contactMenu(contacts);
        }
        catch (FileNotFoundException e)
        {
            System.out.println("The file could not be found. Please try again");
        }
    }

    // -------------
    // Menu methods:
    // -------------

    // Print the sub menu.
    private static void printContactMenu()
    {
        System.out.println("\nList Operation Menu");
        System.out.print("---------\n\n");
        System.out.println("1) view the list");
        System.out.println("2) add an item");
        System.out.println("3) edit an item");
        System.out.println("4) remove an item");
        System.out.println("5) save the current list");
        System.out.println("6) quit to the main menu");
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

    // Print the sub menu.
    private static void printMenu()
    {
        System.out.println("\nMain menu");
        System.out.print("---------\n\n");
        System.out.println("1) create a new list");
        System.out.println("2) load an existing list");
        System.out.println("3) quit");
    }
}
