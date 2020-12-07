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

    // Check if due date is on or after date of creation.
    private static boolean isDueDateValid(String date)
    {
        int year = Integer.parseInt(date.substring(1, 5));
        int day = Integer.parseInt(date.substring(6, 8));
        int month = Integer.parseInt(date.substring(9, 11));

        // Invalid month.
        if (month < 1 || month > 12)
        {
            throw new InvalidDateException("WARNING: invalid due date; task not created");
        }

        // Invalid day.
        if (day < 1 || day > 31)
        {
            throw new InvalidDateException("WARNING: invalid due date; task not created");
        }

        // Invalid year length.
        if (String.valueOf(year).length() > 4)
        {
            throw new InvalidDateException("WARNING: invalid due date; task not created");
        }

        return true;
    }

    // Get description.
    public String getDesc()
    {
        return desc;
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
        if (isDueDateValid(dueDate))
        {
            this.dueDate = dueDate;
        }
        else
        {
            throw new InvalidDateException("Due date is not valid; cannot be before the current date.");
        }
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
