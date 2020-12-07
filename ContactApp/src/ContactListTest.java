import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

class ContactListTest
{

    @Test
    public void addingItemsIncreasesSize()
    {
        ContactList contacts = new ContactList();
        assertEquals(0, contacts.getSize());
        contacts.addContact(new ContactItem("first", "last", "000-000-0000", "email@email.com"));
        assertEquals(1, contacts.getSize());
    }

    @Test
    public void removingItemsDecreasesSize()
    {
        ContactList contacts = new ContactList();
        ContactItem newContact = new ContactItem("first", "last", "000-000-0000", "email@email.com");
        contacts.addContact(newContact);
        ContactItem contact2 = new ContactItem("john", "appleseed", "number", "wow@wow.com");
        contacts.addContact(contact2);
        assertEquals(2, contacts.getSize());
        contacts.removeContact(1);
        assertEquals(1, contacts.getSize());
    }

    @Test
    public void editingItemsFailsWithAllBlankValues()
    {
        assertThrows(IllegalArgumentException.class, () -> {
            ContactList contacts = new ContactList();
            contacts.addContact(new ContactItem("first", "last", "000-000-0000", "email@email.com"));
            contacts.editContactList(0, "", "", "", "");
        });
    }

    @Test
    public void editingItemsFailsWithInvalidIndex()
    {
        assertThrows(IllegalArgumentException.class, () -> {
            ContactList contacts = new ContactList();
            contacts.addContact(new ContactItem("first", "last", "000-000-0000", "email@email.com"));
            contacts.editContactList(0, "", "", "", "");
        });
    }

    @Test
    public void removingItemsFailsWithInvalidIndex(){
        ContactItem item = new ContactItem("first","last","000-000-0000","email@email.com");
        ContactList contact = new ContactList();

        // Insert new contacts.
        contact.addContact(item);
        contact.addContact(item);
        contact.addContact(item);

        assertThrows(IndexOutOfBoundsException.class, () -> contact.removeContact(10));
    }

    @Test
    public void editingSucceedsWithNonBlankValues(){
        ContactItem contactItem = new ContactItem("first","last","111-111-111","test@test.com");
        ContactList contacts = new ContactList();
        contacts.addContact(contactItem);
        contacts.editContactList(0, "First","Last","111-111-1111","test@test.com");

        // Check that conditions are true
        assertEquals("First", contactItem.getFirstName());
        assertEquals("Last", contactItem.getLastName());
        assertEquals("111-111-1111", contactItem.getNumber());
        assertEquals("test@test.com", contactItem.getEmail());
    }

    @Test
    public void editingSucceedsWithBlankFirstName(){
        ContactItem item = new ContactItem("first","last","000-000-0000","email@email.com");
        ContactList  task = new ContactList();
        task.addContact(item);

        // Check assertion.
        assertDoesNotThrow(() -> task.editContactList(0, "","Deer","111-111-1111","email@email.com"));
    }

    @Test
    public void editingSucceedsWithBlankLastName(){
        ContactItem item = new ContactItem("first","last","000-000-0000","random@random.com");
        ContactList  t = new ContactList();
        t.addContact(item);

        // Check assertion.
        assertDoesNotThrow(() -> t.editContactList(0, "Jane","","111-111-1111","email@email.com"));
    }

    @Test
    public void editingSucceedsWithBlankPhone(){
        ContactItem item = new ContactItem("first","last","111-111-1111","email@email.com");
        ContactList  t = new ContactList();
        t.addContact(item);

        // Check assertion.
        assertDoesNotThrow(() -> t.editContactList(0, "John","","","hi@hi.com"));
    }

    @Test
    public void editingSucceedsWithOnlyPhone(){
        ContactItem item = new ContactItem("first","last","111-111-1111","email@email.com");
        ContactList  t = new ContactList();
        t.addContact(item);

        // Check assertion.
        assertDoesNotThrow(() -> t.editContactList(0, "","000-000-0000","",""));
    }

    @Test
    public void editingSucceedsWithOnlyEmail(){
        ContactItem item = new ContactItem("first","last","000-000-0000","email@email.com");
        ContactList  task = new ContactList();
        task.addContact(item);

        // Check assertion.
        assertDoesNotThrow(() -> task.editContactList(0, "", "", "email@email.com", "0"));
    }

    @Test
    public void editingSucceedsWithOnlyFirstName(){
        ContactItem item = new ContactItem("first","last","000-000-0000","email@email.com");
        ContactList  task = new ContactList();
        task.addContact(item);

        // Check assertion.
        assertDoesNotThrow(() -> task.editContactList(0, "","","","0"));
    }

    @Test
    public void editingSucceedsWithOnlyLastName(){
        ContactItem item = new ContactItem("first","last","000-000-0000","email@email.com");
        ContactList  test = new ContactList();
        test.addContact(item);

        // Check assertion.
        assertDoesNotThrow(() -> test.editContactList(0, "Doe","","",""));
    }

    @Test
    public void savedTaskListCanBeLoaded()
    {
        ContactList contacts = new ContactList();
        String filename = "taskListTest.txt";

        // Check assertion.
        assertDoesNotThrow(() -> contacts.loadFile(filename));
    }

    @Test
    public void newListIsEmpty()
    {
        ContactList test = new ContactList();
        assertTrue(test.getSize() == 0);
    }
}
