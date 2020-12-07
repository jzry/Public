public class Pyramid extends Shape3D
{
    private final double length;
    private final double width;
    private final double height;

    public Pyramid(double length, double width, double height)
    {
        this.length = length;
        this.width = width;
        this.height = height;
    }

    // Return name of shape.
    public String getName()
    {
        return "pyramid";
    }

    // Return area.
    public double getArea()
    {
        return ((length * width) + CalcArea(width, height) + CalcArea(length, height));
    }

    // Return volume.
    public double getVolume()
    {
        return ((length * width * height) / 3);
    }

    public double CalcArea(double x, double height)
    {
        return (Math.sqrt(Math.pow((x / 2), 2) + Math.pow(height, 2)));
    }
}