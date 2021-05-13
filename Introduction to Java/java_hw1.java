import java.awt.*;
import java.awt.event.*;

//s1083321
public class java_hw1 extends Frame implements MouseMotionListener, MouseListener
{
	static java_hw1 frm = new java_hw1();
	static int _x, _y, player = 0;
	static int check[] = new int[9];
	static int finish = 0;
	
	public static void main(String args[])
	{
		// init
		for (int i = 0; i < 9; ++i) check[i] = -1;
		frm.addWindowListener(new WindowAdapter(){public void windowClosing(WindowEvent e){System.exit(0);}});		
		frm.setTitle("Tic Tac Toe");
		frm.setSize(680, 680);
		
		frm.addMouseListener(frm);
		frm.addMouseMotionListener(frm);
		
		frm.setVisible(true);
	}
	
	public void paint(Graphics g)
	{
		Graphics2D g2 = (Graphics2D) g;
		g2.setStroke(new BasicStroke(5));
		
		g2.drawLine(220, 20, 220, 620);
		g2.drawLine(420, 20, 420, 620);
		g2.drawLine(20, 220, 620, 220);
		g2.drawLine(20, 420, 620, 420);
	}
	
	public int check_winner()
	{
		for (int i = 0; i < 3; ++i)
		{
			if (check[i * 3] == 0 && check[i * 3 + 1] == 0 && check[i * 3 + 2] == 0) return 0;
			if (check[i] == 0 && check[i + 3] == 0 && check[i + 6] == 0) return 0;
			if (check[i * 3] == 1 && check[i * 3 + 1] == 1 && check[i * 3 + 2] == 1) return 1;
			if (check[i] == 1 && check[i + 3] == 1 && check[i + 6] == 1) return 1;
		}
		if (check[0] == 0 && check[4] == 0 && check[8] == 0) return 0;
		if (check[2] == 0 && check[4] == 0 && check[6] == 0) return 0;
		if (check[0] == 1 && check[4] == 1 && check[8] == 1) return 1;
		if (check[2] == 1 && check[4] == 1 && check[6] == 1) return 1;
		
		return -1;
	}
	
	public void mouseClicked(MouseEvent e)
	{
		if (finish == 1) return;
		
		Graphics g = getGraphics();
		Graphics2D g2 = (Graphics2D) g;
		g2.setStroke(new BasicStroke(3));		
		
		_x = (e.getX() - 20) / 200;
		_y = (e.getY() - 20) / 200;
		
		for (int i = 0; i < 3; ++i) for (int j = 0; j < 3; ++j)
		{
			if (_x == i && _y == j && check[i * 3 + j] == -1)
			{
				if (player == 0)
				{
					g2.setColor(Color.red);
					g2.drawOval(i * 200 + 70 , j * 200 + 70, 100, 100);
				}
				else
				{
					g2.setColor(Color.blue);
					g2.drawLine(i * 200 + 70, j * 200 + 70, i * 200 + 170, j * 200 + 170);
					g2.drawLine(i * 200 + 170, j * 200 + 70, i * 200 + 70, j * 200 + 170);
				}
								
				check[i * 3 + j] = player;
				player ^= 1;
				
				int tmp = check_winner();
				if (tmp == 0)
				{
					System.out.println("O wins.");
					finish = 1;
				}
				else if (tmp == 1)
				{
					System.out.println("X wins.");
					finish = 1;
				}
				else
				{
					int out = 1;
					for (int k = 0; k < 9; ++k) if (check[k] == -1) out = 0;
					if (out == 1) 
					{
						System.out.println("Tie.");
						finish = 1;
					}
				}
				
				break;
			}
		}

	}
	public void mouseMoved(MouseEvent e){}
	public void mouseReleased(MouseEvent e){}
	public void mouseEntered(MouseEvent e){}
	public void mouseExited(MouseEvent e){}
	public void mouseDragged(MouseEvent e){}
	public void mousePressed(MouseEvent e){}
}