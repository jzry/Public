// Calculate the BMI scores and categories.
public class BodyMassIndex
{
    String BMI_category;
    double BMI_score;

    // Quick reference to score and category.
    public BodyMassIndex(double height, double weight)
    {
        this.BMI_score = getBMI_calculator(height, weight);
        this.BMI_category = getBMI_category(height, weight);
    }

    // Get BMI score.
    public static double getBMI_calculator(double height, double weight)
    {
        double score = ((703 * weight) / (height * height));

        // Round to tenths.
        double rounded = Math.round(score * 10)/10.0;
        return rounded;
    }

    // Find BMI category and return that category.
    public static String getBMI_category(double height, double weight)
    {
        String cat;
        double BMI_score;

        // Calculate score.
        BMI_score = getBMI_calculator(height, weight);

        // Get category.
        cat = categoryList(BMI_score);

        return cat;
    }

    // Determine BMI category with BMI score.
    public static String categoryList(double score)
    {
        // Categorizes individual based on weight and height.
        if (score <= 18.5)
        {
            // Under weight.
            return "Underweight";
        }
        else if (score > 18.5 && score <= 24.9)
        {
            // Normal weight.
            return "Normal weight";
        }
        else if (score >= 25.0 && score <= 29.9)
        {
            // Overweight.
            return "Overweight";
        }
        else
        {
            // Obese.
            return "Obese";
        }
    }
}
