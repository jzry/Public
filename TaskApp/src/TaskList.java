import java.util.ArrayList;

public class TaskList
{
    private ArrayList<TaskItem> tasks;

    // Instantiate new tasks object.
    public TaskList()
    {
        this.tasks = new ArrayList<TaskItem>();
    }

    // Return an array list.
    public ArrayList<TaskItem> getTaskList()
    {
        return this.tasks;
    }

    // Add a task.
    public void addTask(TaskItem newTask)
    {
        tasks.add(newTask);
    }

    // Remove a task from a certain index.
    public void removeTask(int index)
    {
        tasks.remove(index);
    }

    // Edit the task list given an index, title, description, and due date.
    public void editTaskList(int index, String title, String desc, String dueDate, boolean status)
    {
        try
        {
            tasks.get(index).setTitle(title);
            tasks.get(index).setDesc(desc);
            tasks.get(index).setDueDate(dueDate);
            tasks.get(index).setStatus(status);
        }
        catch (TaskItem.InvalidDateException e)
        {
            System.out.println("WARNING: invalid due date; task not edited");
        }
        catch (TaskItem.InvalidTitleException e)
        {
            System.out.println("WARNING: title must be at least 1 character long; task not edited");
        }
    }

    // Mark index as complete.
    public void complete(int index)
    {
        tasks.get(index).setStatus(true);
    }

    // Mark index as incomplete.
    public void incomplete(int index)
    {
        tasks.get(index).setStatus(false);
    }

    // Made one for only completed materials.
    // Print out string with proper complete status.
    public String output()
    {
        String outputText = "";

        // Loop through all the items in the list.
        for (int i = 0; i < tasks.size(); i++)
        {
            if (tasks.get(i).getStatus())
            {
                // Print out with *** completed status.
                outputText += String.format("%d) *** %s%n", i, tasks.get(i));
            }
            else
            {
                // Print out without *** completed status.
                outputText += String.format("%d) %s%n", i, tasks.get(i));
            }
        }

        return outputText;
    }
}
