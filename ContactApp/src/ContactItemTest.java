import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

public class ContactItemTest
{
    @Test
    public void creationSucceedsWithBlankEmail()
    {
        assertDoesNotThrow(() -> new ContactItem("first", "last", "000-000-0000", ""));
    }

    @Test
    public void editingSucceedsWithNonBlankValues()
    {
        ContactList contacts = new ContactList();

        // Assert conditions.
        contacts.addContact(new ContactItem("first", "last", "000-000-0000", "email@email.com"));
        contacts.editContactList(0, "first", "last", "000-000-0000", "email@email.com");
    }

    @Test
    public void creationSucceedsWithNonBlankValues()
    {
        assertDoesNotThrow(() -> new ContactItem("first", "last", "000-000-0000", "email@email.com"));
    }

    @Test
    public void creationSucceedsWithBlankFirstName()
    {
        assertDoesNotThrow(() -> new ContactItem("", "last", "000-000-0000", "email@email.com"));
    }

    @Test
    public void creationSucceedsWithBlankPhone()
    {
        assertDoesNotThrow(() -> new ContactItem("first", "last", "", "email@email.com"));
    }

    @Test
    public void editingFailsWithAllBlankValues()
    {
        assertThrows(IllegalArgumentException.class, () -> {
            ContactList contacts = new ContactList();

            // Assert conditions.
            contacts.addContact(new ContactItem("first", "last", "000-000-0000", "email@email.com"));
            contacts.editContactList(0, "", "", "", "");
        });
    }

    @Test
    public void creationFailsWithAllBlankValues()
    {
        assertThrows(IllegalArgumentException.class, () -> new ContactItem("", "", "", ""));
    }

    @Test
    public void creationSucceedsWithBlankLastName()
    {
        assertDoesNotThrow(() -> new ContactItem("first", "", "000-000-0000", "email@email.com"));
    }

    @Test
    public void editingSucceedsWithBlankLastName()
    {
        ContactList contacts = new ContactList();

        // Assert conditions.
        contacts.addContact(new ContactItem("first", "last", "000-000-0000", "email@email.com"));
        contacts.editContactList(0, "first", "", "000-000-0000", "email@address.email");
    }

    @Test
    public void editingSucceedsWithBlankEmail()
    {
        ContactList contacts = new ContactList();

        // Assert conditions.
        contacts.addContact(new ContactItem("first", "last", "000-000-0000", "email@email.com"));
        contacts.editContactList(0, "first", "last", "000-000-0000", "");
    }

    @Test
    public void editingSucceedsWithBlankFirstName()
    {
        ContactList contacts = new ContactList();

        // Assert conditions.
        contacts.addContact(new ContactItem("first", "last", "000-000-0000", "email@email.com"));
        contacts.editContactList(0, "", "last", "000-000-0000", "email@address.email");
    }

    @Test
    public void editingSucceedsWithBlankPhone()
    {
        ContactList contacts = new ContactList();

        // Assert conditions.
        contacts.addContact(new ContactItem("first", "last", "000-000-0000", "email@email.com"));
        contacts.editContactList(0, "first", "last", "", "email@email.com");
    }

    @Test
    public void testToString()
    {
        ContactItem contact = new ContactItem("first", "last", "000-000-0000", "email@email.com");

        // Assert condition.
        assertEquals("Name: first last\n" + "Phone: 000-000-0000\n" + "Email: email@email.com\n", contact.toString());
    }
}
