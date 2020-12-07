public class Square extends Shape2D
{
    private final double side;

    public Square(double side)
    {
        this.side = side;
    }

    // Return name of shape.
    public String getName()
    {
        return "square";
    }

    // Return area.
    public double getArea()
    {
        return Math.pow(side, 2);
    }
}
