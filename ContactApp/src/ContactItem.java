import java.util.Scanner;
import java.time.LocalDate;

public class ContactItem
{
    private String first_name;
    private String last_name;
    private String number;
    private String email;

    // Create the task item.
    public ContactItem(String first_name, String last_name, String number, String email)
    {
        // Set variables.
        this.first_name = first_name;
        this.last_name = last_name;
        this.number = number;
        this.email = email;
    }

    // Overriding the toString function
    public String toString()
    {
        return "Name: " + first_name + " " + last_name + "\n" + "Number: " + number + "\n" +  "Email: " + email;
    }

    // Get the first name.
    public String getFirstName()
    {
        return first_name;
    }

    // Get the first name.
    public String getLastName()
    {
        return last_name;
    }

    // Get the last name.
    public String getTitle()
    {
        return last_name;
    }

    // Get the email.
    public String getEmail()
    {
        return email;
    }

    // Get the number.
    public String getNumber()
    {
        return number;
    }

    // -----------
    // Class setup
    // -----------

    // Set the first name.
    public void setFirstName(String first_name)
    {
        this.first_name = first_name;
    }

    // Set the last name.
    public void setLastName(String last_name)
    {
        this.last_name = last_name;
    }

    // Set the number.
    public void setNumber(String number)
    {
        this.number = number;
    }

    // Set the email.
    public void setEmail(String email)
    {
        this.email = email;
    }

    // ------------------------
    // Invalid Argument Methods
    // ------------------------

    // Inherit class IllegalArgumentException.
    class InvalidTitleException extends IllegalArgumentException
    {
        public InvalidTitleException(String err)
        {
            // Allow exception to be accessed in all files.
            super(err);
        }
    }

    // Inherit class IllegalArgumentException.
    static class InvalidDateException extends IllegalArgumentException
    {
        public InvalidDateException(String err)
        {
            // Allow exception to be accessed in all files.
            super(err);
        }
    }

    // Inherit class IllegalArgumentException.
    class StringIndexOutOfBoundsException extends IllegalArgumentException
    {
        public StringIndexOutOfBoundsException(String err)
        {
            // Allow exception to be accessed in all files.
            super(err);
        }
    }
}
