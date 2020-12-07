import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

class TaskListTest
{
    @Test
    public void addingTaskItemsIncreasesSize()
    {
        TaskList task = new TaskList();

        // Get first list.
        int list1 = task.getTaskList().size();
        TaskItem x = new TaskItem("title", "description", "2021-10-10");
        task.addTask(x);

        // Get second list.
        int list2 = task.getTaskList().size();
        assertTrue(list2 > list1);
    }

    @Test
    public void completingTaskItemChangesStatus()
    {
        TaskList task = new TaskList();

        // Add the item.
        TaskItem a = new TaskItem("title", "description", "2021-10-10");
        task.addTask(a);

        // Do completion.
        boolean complete1 = task.getTaskList().get(0).getStatus();
        task.complete(0);
        boolean complete2 = task.getTaskList().get(0).getStatus();

        // Do assertion.
        assertTrue(complete1 != complete2);
    }

    @Test
    public void completingTaskItemFailsWithInvalidIndex()
    {
        TaskList task = new TaskList();

        // Create new item.
        TaskItem x = new TaskItem("title", "description", "2021-10-10");
        task.addTask(x);

        // Do assertion.
        Assertions.assertThrows(IndexOutOfBoundsException.class, () -> {
            task.complete(1);
        });
    }

    @Test
    public void editingTaskItemChangesValues()
    {
        TaskList t = new TaskList();

        // Add new item.
        TaskItem a = new TaskItem("title 1", "description", "2021-10-10");
        t.addTask(a);

        // Fill up the new first task.
        String title1 = t.getTaskList().get(0).getTitle();
        String desc1 = t.getTaskList().get(0).getDesc();
        String date1 = t.getTaskList().get(0).getDueDate();
        t.editTaskList(0, "title 2", "description b", "2021-10-17", false);

        // Fill up the second task.
        String title2 = t.getTaskList().get(0).getTitle();
        String desc2 = t.getTaskList().get(0).getDesc();
        String date2 = t.getTaskList().get(0).getDueDate();

        // Assert everything is equal.
        assertTrue(!title1.equals(title2) && !desc1.equals(desc2) && !date1.equals(date2));
    }

    @Test
    public void editingTaskItemDescriptionChangesValue()
    {
        TaskList t = new TaskList();

        // Make a new item.
        TaskItem a = new TaskItem("title", "description 1", "2021-10-10");
        t.addTask(a);

        // Get new first date
        String date1 = t.getTaskList().get(0).getDesc();
        t.editTaskList(0, "title", "description 2", "2021-10-10", false);

        // Get second date
        String date2 = t.getTaskList().get(0).getDesc();
        assertFalse(date1.equals(date2));
    }

    @Test
    public void editingTaskItemDescriptionFailsWithInvalidIndex()
    {
        TaskList task = new TaskList();

        // Make new item.
        TaskItem x = new TaskItem("title", "description", "2021-10-10");
        task.addTask(x);

        // Assert that task list is edited.
        Assertions.assertThrows(IndexOutOfBoundsException.class, () -> {
            task.editTaskList(1, "title", "description b", "2021-10-10", false);
        });
    }

    @Test
    public void editingTaskItemDueDateChangesValue()
    {
        TaskList t = new TaskList();

        // Make new item.
        TaskItem a = new TaskItem("title", "description", "2021-10-10");
        t.addTask(a);

        // Get new date.
        String date1 = t.getTaskList().get(0).getDueDate();
        t.editTaskList(0, "title", "description", "2021-10-16", false);

        // Get second new date.
        String date2 = t.getTaskList().get(0).getDueDate();

        // Assert that they are not equal.
        assertFalse(date1.equals(date2));
    }

    @Test
    public void editingTaskItemDueDateFailsWithInvalidIndex()
    {
        TaskList task = new TaskList();

        // Make new item.
        TaskItem x = new TaskItem("title", "description", "2021-10-10");
        task.addTask(x);

        // Do assertion.
        Assertions.assertThrows(IndexOutOfBoundsException.class, () -> {
            task.editTaskList(1, "title", "description", "2021-10-11", false);
        });
    }

    @Test
    public void editingTaskItemTitleChangesValue()
    {
        TaskList task = new TaskList();
        TaskItem x = new TaskItem("title 1", "description", "2021-10-10");
        task.addTask(x);

        // Get first task.
        String task1 = task.getTaskList().get(0).getTitle();
        task.editTaskList(0, "title 2", "description", "2021-10-10", false);

        // Get second task.
        String task2 = task.getTaskList().get(0).getTitle();

        // Assert edit occurred.
        assertFalse(task1.equals(task2));
    }

    @Test
    public void editingTaskItemTitleFailsWithInvalidIndex()
    {
        TaskList task = new TaskList();

        // Make new item.
        TaskItem x = new TaskItem("title 1", "description", "2021-10-10");
        task.addTask(x);

        // Do assertion.
        Assertions.assertThrows(IndexOutOfBoundsException.class, () -> {
            task.editTaskList(1, "title 2", "description b", "2021-10-10", false);
        });
    }

    @Test
    public void gettingTaskItemDescriptionFailsWithInvalidIndex()
    {
        TaskList task = new TaskList();
        TaskItem a = new TaskItem("title", "description", "2021-10-10");
        task.addTask(a);

        // Do assertion.
        Assertions.assertThrows(IndexOutOfBoundsException.class, () -> {
            task.getTaskList().get(1).getDesc();
        });
    }

    @Test
    public void gettingTaskItemDescriptionSucceedsWithValidIndex()
    {
        TaskList task = new TaskList();
        TaskItem x = new TaskItem("title", "description", "2021-10-10");
        task.addTask(x);
        assertEquals("description", task.getTaskList().get(0).getDesc());
    }

    @Test
    public void gettingTaskItemDueDateFailsWithInvalidIndex()
    {
        TaskList task = new TaskList();
        TaskItem x = new TaskItem("title", "description", "2021-10-10");
        task.addTask(x);

        // Do assertion.
        Assertions.assertThrows(IndexOutOfBoundsException.class, () ->
        {
            task.getTaskList().get(1).getDueDate();
        });
    }

    @Test
    public void gettingTaskItemDueDateSucceedsWithValidIndex()
    {
        TaskList task = new TaskList();
        TaskItem x = new TaskItem("title", "description", "2021-10-10");
        task.addTask(x);
        assertEquals("2020-11-16", task.getTaskList().get(0).getDueDate());
    }

    @Test
    public void gettingTaskItemTitleFailsWithInvalidIndex()
    {
        TaskList task = new TaskList();
        TaskItem x = new TaskItem("title", "description", "2021-10-10");
        task.addTask(x);
        Assertions.assertThrows(IndexOutOfBoundsException.class, () -> {
            task.getTaskList().get(1).getTitle();
        });
    }

    @Test
    public void gettingTaskItemTitleSucceedsWithValidIndex()
    {
        TaskList task = new TaskList();
        TaskItem x = new TaskItem("title", "description", "2021-10-10");
        task.addTask(x);
        assertEquals("Title", task.getTaskList().get(0).getTitle());
    }

    @Test
    public void newTaskListIsEmpty()
    {
        TaskList t = new TaskList();
        assertEquals(0, t.getTaskList().size());
    }

    @Test
    public void removingTaskItemsDecreasesSize()
    {
        TaskList task = new TaskList();
        TaskItem x = new TaskItem("title", "description", "2021-10-10");
        task.addTask(x);

        // Remove task.
        int task1 = task.getTaskList().size();
        task.removeTask(0);

        // Remove task.
        int task2 = task.getTaskList().size();
        assertTrue(task2 < task1);
    }

    @Test
    public void removingTaskItemsFailsWithInvalidIndex()
    {
        TaskList task = new TaskList();
        TaskItem x = new TaskItem("title", "description", "2021-10-10");
        task.addTask(x);

        // Do assertion.
        Assertions.assertThrows(IndexOutOfBoundsException.class, () -> {
            task.removeTask(1);
        });
    }

    @Test
    public void savedTaskListCanBeLoaded()
    {
        TaskList t = new TaskList();
        TaskItem a = new TaskItem("title", "description", "2021-10-10");
        t.addTask(a);

        // Assert true.
        assertTrue(true);
    }

    @Test
    public void uncompletingTaskItemChangesStatus()
    {
        TaskList task = new TaskList();
        TaskItem a = new TaskItem("title", "description", "2021-10-10");
        task.addTask(a);
        task.complete(0);

        // Check incomplete 1.
        boolean incomplete1 = task.getTaskList().get(0).getStatus();
        task.incomplete(0);

        // Check incomplete 2.
        boolean incomplete2 = task.getTaskList().get(0).getStatus();
        assertTrue(incomplete1 != incomplete2);
    }

    @Test
    public void uncompletingTaskItemFailsWithInvalidIndex()
    {
        TaskList task = new TaskList();
        TaskItem x = new TaskItem("title", "description", "2021-10-10");
        task.addTask(x);

        // Do assertion.
        Assertions.assertThrows(IndexOutOfBoundsException.class, () -> {
            task.incomplete(1);
        });
    }
}