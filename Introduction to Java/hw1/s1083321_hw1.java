import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class s1083321_hw1 extends Frame implements ActionListener
{
    static s1083321_hw1 frm = new s1083321_hw1();
    static JButton btn[] = new JButton[9];
    static int check[] = new int[9];
    static String str = "O";
    static boolean finish = false;

    public static void main(String args[])
    {
        frm.addWindowListener(new WindowAdapter(){public void windowClosing(WindowEvent e){System.exit(0);}});	
        frm.setTitle("Tic Tac Toe");
        frm.setSize(400, 400);
        for (int i = 0; i < 9; ++i) check[i] = -1;

        GridLayout grid = new GridLayout(3, 3);
        frm.setLayout(grid);
        for (int i = 0; i < 9; ++i)
        {
            frm.add(btn[i] = new JButton());
            btn[i].addActionListener(frm);
        } 

        frm.setVisible(true);
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

    public void actionPerformed(ActionEvent e)
    {
        if (finish == true) return;

        JButton b = (JButton) e.getSource();

        for (int i = 0; i < 9; ++i) if (b == btn[i] && check[i] == -1)
        {
            b.setText(str);
            check[i] = (str == "O" ? 0 : 1);
            str = (str == "O" ? "X" : "O");
        }
        
        int tmp = check_winner();
        if (tmp == 0) System.out.println("O wins.");
        else if (tmp == 1) System.out.println("X wins.");
        else
        {
            for (int k = 0; k < 9; ++k) if (check[k] == -1) return;
            System.out.println("Tie.");
        }

        finish = true;
    }
}