public class Circle extends Shape2D
{
    private final double radius;

    public Circle(double radius)
    {
        this.radius = radius;
    }

    // Return name of shape.
    public String getName()
    {
        return "circle";
    }

    // Return area.
    public double getArea()
    {
        return (Math.PI * Math.pow(radius, 2));
    }
}
