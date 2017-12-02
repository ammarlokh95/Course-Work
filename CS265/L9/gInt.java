import java.io.*;

public class gInt {
	private int real;
	private int comp;
	public gInt(int r)
	{
		real=r;
		comp=0;
	}
	public gInt(int r, int i)
	{
		real=r;
		comp=i;
	}
	public int real()
	{
		return real;
	}
	public int imag()
	{
		return comp;
	}
	public gInt add(gInt rhs)
	{
		int r = this.real() + rhs.real();
		int i = this.imag() + rhs.imag();
		gInt temp = new gInt(r,i);
		return temp;
	}
	public gInt multiply ( gInt rhs)
	{
		int r = ((this.real() * rhs.real()) - (this.imag() * rhs.imag()));
		int i = ((this.real() * rhs.imag()) + (this.imag() * rhs.real()));
		gInt temp = new gInt(r,i);
		return temp;
	}
	public float norm()
	{
		double d1 = (this.real()*this.real())+(this.imag()*this.imag());
		d1 = Math.sqrt(d1);
		return (float)d1;
	}

	public static void main(String [] arg)
	{
		gInt g1 = new gInt(3);
		gInt g2 = new gInt(-1,2);
		gInt g5 = new gInt(1,1);
		gInt g3 = g1.add(g2);
		System.out.println("g3 = "+ g3.real() + " + " + g3.imag() +"i");
		gInt g4 = g2.multiply(g1);
		g3 = g2.add(g5);
		float f = g2.norm();
		System.out.println("g3 = "+ g3.real() + " + " + g3.imag() +"i");
		System.out.println("g4 = " + g4.real() + " + " + g4.imag() + "i");
		g4 = g2.multiply(g5);
		System.out.println("g4 = " + g4.real() + " + " + g4.imag() + "i");
		System.out.println("g4 distance = " + f);
	}
}
