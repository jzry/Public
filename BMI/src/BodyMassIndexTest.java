import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

class BodyMassIndexTest
{
    @Test
    public void UnderweightTest()
    {
        BodyMassIndex c = new BodyMassIndex(93, 145);

        // Assert to check expected value.
        assertEquals("Underweight", c.getBMI_category(93, 145));
    }

    @Test
    public void NormalWeightTest()
    {
        BodyMassIndex d = new BodyMassIndex(76, 212);

        // Assert to check expected value.
        assertEquals("Normal Weight", d.getBMI_category(76, 212));
    }

    @Test
    public void OverweightCatTest()
    {
        BodyMassIndex e = new BodyMassIndex(71, 234);

        // Assert to check expected value.
        assertEquals("Overweight", e.getBMI_category(71, 234));
    }

    @Test
    public void GetBMI_calculatorTest()
    {
        BodyMassIndex a = new BodyMassIndex(61,241);

        // Assert to check expected value.
        assertEquals(45.5, a.getBMI_calculator(61, 241));
    }

    @Test
    public void RoundingGetBMI_calculatorTest()
    {
        BodyMassIndex b = new BodyMassIndex(99, 147);

        // Assert to check expected value.
        assertEquals(10.5, b.getBMI_calculator(99, 147));
    }

    @Test
    public void ObeseCatTest()
    {
        BodyMassIndex f = new BodyMassIndex(38, 297);

        // Assert to check expected value.
        assertEquals("Obesity", f.getBMI_category(38, 297));
    }
}