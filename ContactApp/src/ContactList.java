import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;

public class ContactList
{
    private ArrayList<ContactItem> contacts;

    // Local Scanner object.
    private static Scanner in = new Scanner(System.in);

    // Instantiate new contact object.
    public ContactList()
    {
        this.contacts = new ArrayList<ContactItem>();
    }

    // Return an array list.
    public ArrayList<ContactItem> getContactList()
    {
        return this.contacts;
    }

    public int getSize()
    {
        return contacts.size();
    }

    // Add a contact.
    public void addContact(ContactItem newContact)
    {
        contacts.add(newContact);
    }

    // Remove a contact from a certain index.
    public void removeContact(int index)
    {
        contacts.remove(index);
    }

    // Edit the contact list.
    public void editContactList(int index, String first_name, String last_name, String number, String email)
    {
        contacts.get(index).setFirstName(first_name);
        contacts.get(index).setLastName(last_name);
        contacts.get(index).setNumber(number);
        contacts.get(index).setEmail(email);
    }

    public String toString(ContactList contact)
    {
        return contact.toString();
    }

    // Made one for only completed materials.
    // Print out string with proper complete status.
    public String output()
    {
        String outputText = "";

        // Loop through all the items in the list.
        for (int i = 0; i < contacts.size(); i++)
        {
            // Get appropriate index message.
            outputText += String.format("%d) %s\n", i, contacts.get(i));
        }

        return outputText;
    }

    // Load a file into the program.
    static void loadFile(String fileName)
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
        }
        catch (FileNotFoundException e)
        {
            System.out.println("The file could not be found. Please try again");
        }
    }
}
