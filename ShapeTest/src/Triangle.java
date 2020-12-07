public class Triangle extends Shape2D
{
    private final double base;
    private final double height;

    public Triangle(double base, double height)
    {
        this.base = base;
        this.height = height;
    }

    // Return name of shape.
    public String getName()
    {
        return "triangle";
    }

    // Return area.
    public double getArea()
    {
        return ((base * height) / 2);
    }
}