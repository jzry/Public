// This program allows the creation of Binary Search Trees (BST).

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;

public class main
{
    // Checks if a string is alphabetical.
    static boolean isAlpha(String name)
    {
        char[] chars = name.toCharArray();

        for (char c : chars)
        {
            if(!Character.isLetter(c))
            {
                return false;
            }
        }

        return true;
    }

    // Main file for terminal.
    public static void main(String[] args) throws FileNotFoundException
    {
        int i = 0;

        // Character for storing command.
        char instruction = 0;
        // Integer for storing data.
        int num = 0;

        // Create an instance of a BST.
        BST tree = new BST();

        // Read the file.
        File file = new File( args[ 0 ] );
        Scanner input = new Scanner(file);
        ArrayList<String> line = new ArrayList<String>();

        while (input.hasNextLine())
        {
            line.add(input.nextLine());
        }

        // Make a string to hold the commands.
        String[] commands = new String[line.size()];
        String temp;

        for (i = 0; i < line.size(); i++)
        {
            commands[i] = line.get(i);

            temp = commands[i];
            String[] parsed = temp.split(" ");

            for (String t : parsed)
            {
                num = Integer.MIN_VALUE;

                if (Character.isLetter(t.charAt(0)))
                {
                    instruction = t.charAt(0);
                }

                if (!isAlpha(t))
                {
                    num = Integer.parseInt(t);                  
                }

                // For input options.

                if (instruction == 'i' && num != Integer.MIN_VALUE)
                {
                    tree.insert(num);
                }

                // Delete data.
                else if (instruction == 'd' && num != Integer.MIN_VALUE)
                {
                    tree.delete(num);
                }

                // Search for data.
                else if (instruction == 's' && num != Integer.MIN_VALUE)
                {
                    tree.search(num);
                }

                // Print BST inorder.
                else if (instruction == 'p')
                {
                    tree.print();
                }

                // Quit program.
                else if (instruction == 'q')
                {
                    break;
                }
            }
        }
    }
}

// This defines the Node class.
class Node
{
    int data;
    Node left, right;

    // Data passed.
    public Node(int data)
    {
        this.data = data;
        right = null;
        left = null;
    }

    // No data is passed.
    public Node()
    {
        this.data = 0;
        right = null;
        left = null;
    }
}


class BST extends Node
{
    // Root instantiation.
    Node root = null;

    // Insertion shortcut.
    void insert(int key)
    {
        root = insertKey(root, key);
    }

    // Recursive insertion method.
    Node insertKey(Node root, int key)
    {
        // Returns new node if trees are empty.
        if (root == null)
        {
            root = new Node(key);
            return root;
        }

        // Key is less that current node data.
        if (key < root.data)
        {
            root.left = insertKey(root.left, key);
        }

        // Key is greater than or equal to current node data.
        else
        {
            root.right = insertKey(root.right, key);
        }

        return root;
    }

    // Delete a node from the BST.
    void delete(int key)
    {
        root = deleteHelper(root, key);
    }

    // Recursive delete function.
    Node deleteHelper(Node root, int key)
    {
        Node temp;

        // Tree is empty.
        if (root == null)
        {
            System.out.printf("command-> d %d: integer %d NOT found - NOT deleted\n", key, key);
            return null;
        }
        else if (key < root.data)
        {
            root.left = deleteHelper(root.left, key);
        }
        else if (key > root.data)
        {
            root.right = deleteHelper(root.right, key);
        }
        else
        {
            if (root.left == null && root.right == null)
            {
                // Contains no children, node is simply deleted.
                root = null;
            }
            else if (root.right == null)
            {
                // Has only a left child. It moves up to take its parents place.
                temp = root.left;
                root = temp;
                root.left = null;
            }
            else if (root.left == null)
            {
                // Has only a right child. It moves up to take its parents place.
                temp = root.right;
                root = temp;
                root.right = null;
            }
            else
            {
                // Node has two children.
                // We find the max find the max value in
                // the left subtree and move it up to this node.
                root.data = find_max(root.left);
                root.left = deleteHelper(root.left, root.data);
            }
        }

        return root;
    }

    // Finds the max value in a BST. Assumes root is non-null.
    int find_max(Node root)
    {
        if (root.right == null)
            return root.data;
        else
            return find_max(root.right);
    }

    // Finds the key in the binary search tree.
    boolean search(int key)
    {
        root = searchHelper(root, key);
        if (root != null)
        {
            return true;
        }
        else
        {
            System.out.printf("command-> s %d: integer %d NOT found\n", key, key);
            return false;
        }
    }

    // Recursive search helper.
    Node searchHelper(Node root, int key)
    {
        if (root == null)
        {
            // Tree doesn't exist.
            return null;
        }

        if (root.data == key)
        {
            // Key is at the root.
            return root;
        }

        if (key < root.data)
        {
            // Key is less than the root.
            return searchHelper(root.left, key);
        }

        return searchHelper(root.right, key);
    }

    // Required function.
    static void complexityIndicator()
    {
        System.err.println("je942968@eustis:~/COP3503$ je942968;1.0;10.0");
    }

    // Returns the number of nodes in a binary tree.
    static int countChildren(Node root)
    {
        if (root == null)
        {
            return 0;
        }

        return 1 + countChildren(root.left) + countChildren(root.right);
    }

    // Required function. Returns the height of a binary tree.
    static int getDepth(Node root)
    {
        if (root == null)
            return -1;

        return 1 + Math.max(getDepth(root.left), getDepth(root.right));
    }

    // Print the BST data inorder.
    void inorder(Node root)
    {
        if (root == null)
            return;

        inorder(root.left);
        System.out.print(root.data + " ");
        inorder(root.right);
    }

    // Gateway function for inorder().
    void print()
    {
        if (root == null)
        {
            return;
        }

        inorder(root);
    }
}
