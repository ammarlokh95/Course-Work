import junit.framework.*;

public class gIntTest extends TestCase {
	private gInt g1;
	private gInt g2;
	private gInt g3;

	public gIntTest(String name)
	{
		super(name);
	}

	protected void setUp()
	{
		g1= new gInt(3);
		g2= new gInt(-1,2);
		g3= new gInt(1,1);
	}
	public static Test suite() {
		return new TestSuite(gIntTest.class);
	}
	public void testEqual() {
		gInt expected = new gInt(-1,2);
		assertEquals(expected.real(),g2.real());
		assertEquals(expected.imag(),g2.imag());
		assertEquals(g2,g2);
		assertNotSame(expected,g2);
	}
	public void testAdd()
	{
		gInt result= g1.add(g2);
		gInt expected= new gInt(2,2);
		assertNotNull(result);
		assertEquals(expected.real(),result.real());
		assertEquals(expected.imag(),result.imag());

		gInt result2=g2.add(g3);
		assertNotNull(result2);
		gInt expected2= new gInt(0,3);
		Assert.assertEquals(result2,result2);
	}
	public void testReal()
	{
		int exp1=3;
		int exp2=-1;
		assertEquals(exp1,g1.real());
		assertEquals(exp2,g2.real());
	}
	public void testImag()
	{
		int exp1=0;
		int exp2=2;
		assertEquals(exp1,g1.imag());
		assertEquals(exp2,g2.imag());
	}
	public void testMult()
	{
		gInt exp1= new gInt(-3,6);
		gInt result= g2.multiply(g1);

		assertNotNull(exp1);
		assertNotNull(result);

		assertEquals(exp1.real(),result.real());
		assertEquals(exp1.imag(),result.imag());
		gInt result2=g2.multiply(g3);
		gInt exp2=new gInt(-3,1);
		assertEquals(exp2.real(),result2.real());
		assertEquals(exp2.imag(),result2.imag());
	}
	public void testNorm()
	{
		float f1=3;
		float f2=(float)Math.sqrt(5.0);

		assertEquals(f1,g1.norm());
		assertEquals(f2,g2.norm());
	}
	public static void main(String [] arg)
	{
		junit.textui.TestRunner.run(suite());
	}
}

		

