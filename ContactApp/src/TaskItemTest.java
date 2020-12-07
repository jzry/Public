import org.junit.Test;
import org.junit.jupiter.api.Assertions;
import static org.junit.jupiter.api.Assertions.*;

class TaskItemTest
{
    @Test
    public void creatingTaskItemFailsWithInvalidTitle() {
        Assertions.assertThrows(TaskItem.InvalidTitleException.class, () -> {
            new TaskItem("", "desc", "2021-09-09");
        });
    }

    @Test
    public void creatingTaskItemSucceedsWithValidDueDate() {
        TaskItem newTask = new TaskItem("test", "desc", "2021-09-09");
        assertEquals("2021-09-09", newTask.getDueDate());
    }

    @Test
    public void creatingTaskItemFailsWithInvalidDueDate()
    {
        Assertions.assertThrows(TaskItem.InvalidDateException.class, () -> {
            new TaskItem("test", "description", "2021-04-33");
        });
    }

    @Test
    public void creatingTaskItemSucceedsWithValidTitle()
    {
        TaskItem task = new TaskItem("test", "desc", "2021-09-09");
        assertEquals("test", task.getTitle());
    }

    @Test
    public void settingTaskItemDueDateFailsWithInvalidDate()
    {
        TaskItem task = new TaskItem("test", "desc", "2021-09-09");

        // Do assertion.
        Assertions.assertThrows(TaskItem.InvalidDateException.class, () -> {
            task.setDueDate("2020-10-10");
        });
    }

    @Test
    public void settingTaskItemDueDateSucceedsWithValidDate() {
        TaskItem newTask = new TaskItem("test", "description", "2021-09-09");
        newTask.setDueDate("2021-12-12");

        // Do assertion.
        assertEquals("2021-12-12", newTask.getDueDate());
    }

    @Test
    public void settingTaskItemTitleSucceedsWithValidTitle() {
        TaskItem task = new TaskItem("test", "desc", "2021-09-09");
        task.setTitle("new title");

        // Do assertion.
        assertEquals("new title", task.getTitle());
    }

    @Test
    public void settingTaskItemTitleFailsWithInvalidTitle() {
        TaskItem task = new TaskItem("test", "desc", "2021-09-09");

        // Do assertion.
        Assertions.assertThrows(TaskItem.InvalidTitleException.class, () -> {
            task.setTitle("");
        });
    }

}
