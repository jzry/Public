import org.junit.Assert;
import org.junit.Test;
import org.junit.jupiter.api.Assertions;
import static org.junit.jupiter.api.Assertions.*;

class TaskItemTest
{
    @Test
    public void creatingTaskItemFailsWithInvalidDueDate()
    {
        Assertions.assertThrows(TaskItem.InvalidDateException.class, () -> {
            new TaskItem("test", "description", "2021-04-33");
        });
    }

    @Test
    public void creatingTaskItemFailsWithInvalidTitle() {
        Assertions.assertThrows(TaskItem.InvalidTitleException.class, () -> {
            new TaskItem("", "description", "2021-10-10");
        });
    }

    @Test
    public void creatingTaskItemSucceedsWithValidDueDate() {
        TaskItem task = new TaskItem("test", "description", "2021-10-10");
        assertEquals("2020-10-10", task.getDueDate());
    }

    @Test
    public void creatingTaskItemSucceedsWithValidTitle()
    {
        TaskItem task = new TaskItem("test", "description", "2021-10-10");
        assertEquals("test", task.getTitle());
    }

    @Test
    public void settingTaskItemDueDateFailsWithInvalidDate()
    {
        TaskItem task = new TaskItem("test", "description", "2021-10-10");

        // Do assertion.
        Assertions.assertThrows(TaskItem.InvalidDateException.class, () -> {
            task.setDueDate("2020-10-10");
        });
    }

    @Test
    public void settingTaskItemDueDateSucceedsWithValidDate() {
        TaskItem task = new TaskItem("test", "description", "2021-10-10");
        task.setDueDate("2020-10-10");

        // Do assertion.
        assertEquals("2020-10-10", task.getDueDate());
    }

    @Test
    public void settingTaskItemTitleFailsWithInvalidTitle() {
        TaskItem task = new TaskItem("test", "description", "2021-10-10");

        // Do assertion.
        Assertions.assertThrows(TaskItem.InvalidTitleException.class, () -> {
            task.setTitle("");
        });
    }

    @Test
    public void settingTaskItemTitleSucceedsWithValidTitle() {
        TaskItem task = new TaskItem("test", "description", "2021-10-10");
        task.setTitle("title");

        // Do assertion.
        assertEquals("title", task.getTitle());
    }
}
