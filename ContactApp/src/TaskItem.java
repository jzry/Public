import java.util.Scanner;
import java.time.LocalDate;

public class TaskItem
{
    private String title;
    private String desc;
    private String dueDate;
    private boolean status;

    // Create the task item.
    public TaskItem(String title, String desc, String date)
    {
        if (title.length() < 1)
        {
            throw new IllegalArgumentException("WARNING: title must be at least 1 character long; task not created.");
        }

        // Set variables.
        setDueDate(date);
        this.title = title;
        this.desc = desc;
    }

    // Get description.
    public String getDesc()
    {
        return desc;
    }

    // Overriding the toString function
    public String toString()
    {
        return "[" + dueDate + "] " + title + ": " +  desc;
    }

    // Get title.
    public String getTitle()
    {
        return title;
    }

    // Get due date.
    public String getDueDate()
    {
        return dueDate;
    }

    // Get the completion status.
    public boolean getStatus()
    {
        return status;
    }

    // Set the description.
    public void setDesc(String desc)
    {
        this.desc = desc;
    }

    // Set the due date.
    public void setDueDate(String dueDate)
    {
        this.dueDate = dueDate;
    }

    // Set the description.
    public void setTitle(String desc)
    {
        this.title = title;
    }

    // Set the completion status.
    public void setStatus(boolean status)
    {
        this.status = status;
    }

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
