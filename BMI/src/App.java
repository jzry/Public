import java.util.ArrayList;
import java.util.Scanner;

// Application entry point.
public class App
{
    public static void main(String[] args)
    {
        ArrayList<BodyMassIndex> bmiData = new ArrayList<BodyMassIndex>();

        while (moreInput())
        {
            double height = getUserHeight();
            double weight = getUserWeight();

            BodyMassIndex bmi = new BodyMassIndex(height, weight);
            bmiData.add(bmi);

            displayBmiInfo(bmi);
        }

        displayBmiStatistics(bmiData);
    }

    // Local Scanner object.
    private static Scanner in = new Scanner(System.in);

    // Retrieve double input.
    public static double doubleInput(String prompt)
    {
        // Print out prompt
        System.out.println(prompt);

        // Get double.
        double value = in.nextDouble();

        // Clear newline character.
        String clear = in.nextLine();
        return value;
    }

    // Retrieve string input.
    public static String stringInput(String prompt)
    {
        System.out.println(prompt);
        return in.nextLine();
    }

    // Prompt user 'Y' or 'N' and return appropriate value in bool.
    public static boolean moreInput()
    {
        String response = stringInput("Would you like to enter more info? Enter 'Y' to enter more input, 'N' to exit.");

        // Return either true or false based on response.
        if (response.equals("Y")) {
            return true;
        }

        return false;
    }

    // Get user height in inches.
    public static double getUserHeight()
    {
        double height;

        height = doubleInput("Enter height in inches: ");

        // Exits once user enters valid height.
        while (height <= 0)
        {
            if (height > 0)
            {
                // Allows program to exit.
                break;
            }

            height = doubleInput("Height must be a positive number.");
        }

        return height;
    }

    // Get user weight in pounds.
    public static double getUserWeight()
    {
        double weight;

        weight = doubleInput("Enter weight in pounds: ");

        // Exits once user enters valid height.
        while (weight <= 0)
        {
            if (weight > 0)
            {
                // Allows program to exit.
                break;
            }

            weight = doubleInput("Weight must be a positive number.");
        }

        return weight;
    }

    // Print BMI score and category using data contained in BodyMassIndex Object.
    public static void displayBmiInfo(BodyMassIndex bmi)
    {
        // Print score.
        System.out.println("BMI Score: " + bmi.BMI_score);

        // Print category.
        System.out.println("BMI Category: " + bmi.BMI_category);
    }

    public static void displayBmiStatistics(ArrayList<BodyMassIndex> bmiData)
    {
        double sum = 0, average, counter = 0;

        // Get average
        for (int i = 0; i < bmiData.size(); i++)
        {
            sum += bmiData.get(i).BMI_score;
            counter++;
        }

        average = (sum / counter);

        // Print out average BMI score.
        System.out.printf("Average BMI score: %.2f\n", average);
    }
}
