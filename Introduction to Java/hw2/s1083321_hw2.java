import java.awt.*;
import java.awt.event.*;

public class s1083321_hw2 extends Frame implements MouseMotionListener, MouseListener, ActionListener
{
    static s1083321_hw2 frm = new s1083321_hw2();

    static Label lb[] = new Label[5];
    static Button color_btn[] = new Button[5];
    static Button stroke_btn[] = new Button[2];
    static Button back_btn[] = new Button[2];
    static Button type_btn[] = new Button[3];
    static Button reset_btn = new Button("reset");

    static Color cl = Color.black;
    static Color black = new Color(0, 0, 0);
    static Color white = new Color(255, 255, 255);

    static int St = 1;
    static int x1, y1;
    static int type = 1;

    public static void set_color()
    {
        lb[0] = new Label("Color");
        lb[0].setBounds(10, 40, 40, 20);
        frm.add(lb[0]);

        for (int i = 0; i < 5; ++i)
        {
            color_btn[i] = new Button();
            color_btn[i].setBounds(80 + 20 * i * 2, 40, 40, 20);
            color_btn[i].addActionListener(frm);
            frm.add(color_btn[i]);
        }

        color_btn[0].setBackground(Color.black);
        color_btn[1].setBackground(Color.red);
        color_btn[2].setBackground(Color.yellow);
        color_btn[3].setBackground(Color.green);
        color_btn[4].setBackground(Color.blue);
    }

    public static void set_stroke()
    {
        lb[1] = new Label("Stroke");
        lb[1].setBounds(10, 70, 40, 20);
        frm.add(lb[1]);

        stroke_btn[0] = new Button("+");
        stroke_btn[1] = new Button("-");

        for (int i = 0; i < 2; ++i)
        {
            stroke_btn[i].setBounds(80 + 20 * i * 2, 70, 40, 20);
            stroke_btn[i].addActionListener(frm);
            frm.add(stroke_btn[i]);
        }
    }

    public static void set_back()
    {
        lb[2] = new Label("Back");
        lb[2].setBounds(10, 100, 40, 20);
        frm.add(lb[2]);

        back_btn[0] = new Button("white");
        back_btn[1] = new Button("black");

        for (int i = 0; i < 2; ++i)
        {
            back_btn[i].setBounds(80 + 20 * i * 2, 100, 40, 20);
            back_btn[i].addActionListener(frm);
            frm.add(back_btn[i]);
        }
    }

    public static void set_type()
    {
        lb[3] = new Label("Type");
        lb[3].setBounds(10, 130, 40, 20);
        frm.add(lb[3]);

        type_btn[0] = new Button("line");
        type_btn[1] = new Button("Square");
        type_btn[2] = new Button("Circle");

        for (int i = 0; i < 3; ++i)
        {
            type_btn[i].setBounds(80 + 20 * i * 2, 130, 40, 20);
            type_btn[i].addActionListener(frm);
            frm.add(type_btn[i]);
        }
    }

    public static void set_reset()
    {
        lb[4] = new Label("Reset");
        lb[4].setBounds(10, 160, 40, 20);
        frm.add(lb[4]);

        reset_btn.setBounds(80, 160, 40, 20);
        reset_btn.addActionListener(frm);
        frm.add(reset_btn);
    }

    public static void main(String args[])
    {
        frm.addWindowListener(new WindowAdapter(){public void windowClosing(WindowEvent e){System.exit(0);}});	
        frm.setTitle("painter");
        frm.setSize(1000, 1000);
        frm.setLayout(null);

        frm.addMouseListener(frm);
        frm.addMouseMotionListener(frm);

        set_color();
        set_stroke();
        set_back();
        set_type();
        set_reset();

        frm.setVisible(true);
    }

    public void actionPerformed(ActionEvent e)
    {
        Button b = (Button) e.getSource();

        if (b == color_btn[0]) cl = Color.black;
        else if (b == color_btn[1]) cl = Color.red;
        else if (b == color_btn[2]) cl = Color.yellow;
        else if (b == color_btn[3]) cl = Color.green;
        else if (b == color_btn[4]) cl = Color.blue;

        if (b == stroke_btn[0])
        {
            ++St;
            if (St > 5) St = 5;
        }
        else if (b == stroke_btn[1])
        {
            --St;
            if (St < 1) St = 1;
        }

        Graphics2D g = (Graphics2D)getGraphics();       
        if (b == back_btn[0])
        {
            g.setColor(white);
            g.fillRect(0, 0, getWidth(), getHeight());
        }
        else if (b == back_btn[1])
        {
            g.setColor(black);
            g.fillRect(0, 0, getWidth(), getHeight());
        }

        if (b == type_btn[0]) type = 1;
        else if (b == type_btn[1]) type = 2;
        else if (b == type_btn[2]) type = 3;

        if (b == reset_btn)
        {
            cl = Color.black;
            St = 1;
            type = 1;
            g.setColor(white);
            g.fillRect(0, 0, getWidth(), getHeight());
        }
    }

    public void mousePressed(MouseEvent e)
    {
        x1 = e.getX();
        y1 = e.getY();
    }

    public void mouseDragged(MouseEvent e)
     {
        Graphics2D g = (Graphics2D)getGraphics();
        int x2 = e.getX();
        int y2 = e.getY();

        g.setColor(cl);
        g.setStroke(new BasicStroke(St));

        if (type == 1) g.drawLine(x1, y1, x2, y2);
        else if (type == 2) g.fillRect(x1, y1, x2-x1, y2-y1);
        else g.fillOval(x1, y1, Math.abs(x2-x1), Math.abs(y2-y1));

        x1 = x2;
        y1 = y2;
    }

    public void mouseMoved(MouseEvent e){}
    public void mouseReleased(MouseEvent e){}
    public void mouseEntered(MouseEvent e){}
    public void mouseExited(MouseEvent e){}
    public void mouseClicked(MouseEvent e){}
}