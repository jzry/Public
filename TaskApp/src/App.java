import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
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
            printMenu();

            input = getInput();

            // Enter the option menu loop.
            if (input == 1)
            {
                // Instantiate a new encapsulated task list object.
                TaskList tasks = new TaskList();
                System.out.println("new task list has been created\n");

                // Open option menu loop.
                optionMenu(tasks);
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

    // Print the main menu.
    private static void printMenu()
    {
        System.out.println("Main menu");
        System.out.print("---------\n\n");
        System.out.println("1) create a new list");
        System.out.println("2) load an existing list");
        System.out.println("3) quit");
    }

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
        TaskList tasks = new TaskList();

        try
        {
            // Make a new file object.
            File file = new File(fileName);

            // Create a scanner to read the file.
            Scanner reader = new Scanner(file);

            int count = 0;

            while (reader.hasNextLine())
            {
                // Open and read a task file.
                String taskLine = reader.nextLine();

                // Get proper tasks.
                String title = taskLine.substring(taskLine.indexOf("]")+2, taskLine.indexOf(":"));
                String desc = taskLine.substring(taskLine.indexOf(":")+2);
                String dueDate = taskLine.substring(taskLine.indexOf("[")+1, taskLine.indexOf("]"));

                // Send them to new task list.
                TaskItem newTask = new TaskItem(title, desc, dueDate);
                tasks.addTask(newTask);

                // Change the
                if (taskLine.contains("***"))
                {
                    tasks.complete(count);
                }

                // Increment count line.
                count++;
            }

            // Close the file reader.
            reader.close();

            System.out.println("Task list has been loaded from " + fileName);

            // Load the looping option menu.
            optionMenu(tasks);
        }
        catch (FileNotFoundException e)
        {
            System.out.println("The file could not be found. Please try again");
        }
    }

    // Print the option menu.
    private static void printOptionMenu()
    {
        System.out.println("List Operation Menu");
        System.out.print("---------\n\n");
        System.out.println("1) view the list");
        System.out.println("2) add an item");
        System.out.println("3) edit an item");
        System.out.println("4) remove an item");
        System.out.println("5) mark an item as completed");
        System.out.println("6) unmark an item as completed");
        System.out.println("7) save the current list");
        System.out.println("8) quit to the main menu");
    }

    // Enter the option menu.
    private static void optionMenu(TaskList tasks)
    {
        int input;

        // Loops until break statement is activated.
        while (true)
        {
            printOptionMenu();

            input = getInput();

            // The input actions.
            if (input == 1)
                viewTasks(tasks);
            else if (input == 2)
                addTask(tasks);
            else if (input == 3)
                editTask(tasks);
            else if (input == 4)
                removeTask(tasks);
            else if (input == 5)
                markComplete(tasks);
            else if (input == 6)
                markIncomplete(tasks);
            else if (input == 7)
                saveList(tasks);
            else if (input == 8)
                break;
            else
                System.out.println("Commands 1 through 8 only. Try again.");
        }
    }

    // Print out task list.
    private static void viewTasks(TaskList tasks)
    {
        System.out.print("\nCurrent Tasks\n----------\n\n");
        System.out.println(tasks);
        tasks.output();
    }

    // Add a task into a TaskList object.
    private static void addTask(TaskList tasks)
    {
        // Get data for task object.
        in.nextLine();
        String title = getString("Task title: ");
        String desc = getString("Task description: ");
        String date = getString("Task due date (YYYY-MM-DD): ");

        // Send input to be placed in new TaskItem object.
        try
        {
            TaskItem newTask = new TaskItem(title, desc, date);
            tasks.addTask(newTask);
        }
        catch (StringIndexOutOfBoundsException e)
        {
            System.out.println("WARNING: string index out of bounds; task not created");
        }
        catch (TaskItem.InvalidDateException e)
        {
            System.out.println("WARNING: invalid due date; task not created");
        }
        catch (TaskItem.InvalidTitleException e)
        {
            System.out.println("WARNING: title must be at least 1 character long; task not created");
        }
    }

    // Display message before user input.
    private static int taskPrompt(TaskList tasks, String status, String action)
    {
        System.out.println("\n" + status + " Tasks\n----------\n");
        System.out.println(tasks);
        System.out.print("Which task will you " + action + "? ");
        return getInput();
    }

    // Edit the task at the selected index.
    private static void editTask(TaskList tasks)
    {
        try
        {
            in.nextLine();
            int index = taskPrompt(tasks, "Current", "edit");
            in.nextLine();

            // Enter new information for task and send to be edited.
            String title = getString("Enter a new title for task " + index + ": ");
            String desc = getString("Enter a new description for task " + index + ": ");
            String date = getString("Enter a new task due date (YYYY-MM-DD) for task " + index + ": ");
            tasks.editTaskList(index, title, desc, date, false);
        }
        catch (IndexOutOfBoundsException e)
        {
            System.out.println("WARNING: invalid task choice; task cannot be edited");
        }
    }

    // Remove the task at the selected index.
    private static void removeTask(TaskList tasks)
    {
        try
        {
            in.nextLine();
            int index = taskPrompt(tasks, "Current", "remove");
            tasks.removeTask(index);
        }
        catch (IndexOutOfBoundsException e)
        {
            System.out.println("WARNING: invalid task choice; task cannot be removed");
        }
    }

    // Mark the task at selected index as completed.
    private static void markComplete(TaskList tasks)
    {
        try
        {
            in.nextLine();
            int index = taskPrompt(tasks, "Uncompleted", "mark as completed");
            tasks.complete(index);
        }
        catch (IndexOutOfBoundsException e)
        {
            System.out.println("WARNING: invalid task choice; task cannot be marked as completed");
        }
    }

    // Mark the task at the selected index as incomplete.
    private static void markIncomplete(TaskList tasks)
    {
        try
        {
            in.nextLine();
            int index = taskPrompt(tasks,"Completed", "unmark as completed");
            tasks.incomplete(index);
        }
        catch (IndexOutOfBoundsException e)
        {
            System.out.println("WARNING: invalid task choice; task cannot be unmarked as completed");
        }
    }

    // Make a file and save task contents into it.
    private static void makeFile(TaskList tasks, String fileName) throws IOException
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
        writer.write(tasks.output());

        // Close the input stream.
        writer.close();

        System.out.println("task list has been saved");
    }

    // Save the current list to a file.
    private static void saveList(TaskList tasks)
    {
        in.nextLine();
        String fileName = getString("Enter the filename to save as: ");

        try
        {
            // Make a new file to save the task list to.
            makeFile(tasks, fileName);
        }
        catch (IOException e)
        {
            System.out.println("An error occurred.");
        }
    }
}


