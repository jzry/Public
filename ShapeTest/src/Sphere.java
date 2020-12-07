public class Sphere extends Shape3D
{
    private final double radius;

    public Sphere(double radius)
    {
        this.radius = radius;
    }

    // Return name of shape.
    public String getName()
    {
        return "sphere";
    }

    // Return area.
    public double getArea()
    {
        return (4 * Math.PI * Math.pow(radius, 2));
    }

    // Return volume.
    public double getVolume()
    {
        return ((4 * Math.PI * Math.pow(radius, 3)) / 3);
    }
}