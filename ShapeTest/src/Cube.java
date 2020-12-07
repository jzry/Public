public class Cube extends Shape3D
{
    private final double side;

    public Cube(double side)
    {
        this.side = side;
    }

    // Return name of shape.
    public String getName()
    {
        return "cube";
    }

    // Return area.
    public double getArea()
    {
        return (6 * Math.pow(side, 2));
    }

    // Return volume.
    public double getVolume()
    {
        return Math.pow(side, 3);
    }
}