import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.io.*;
import javax.imageio.*;
import java.util.Random;

public class final_project extends JFrame implements ActionListener, MouseMotionListener, MouseListener
{
    static final_project frm = new final_project();
    static JButton player  = new JButton("vs player");
    static JButton computer = new JButton("vs computer");
    static JButton restart = new JButton("restart");
    static JButton theme = new JButton("theme");
    static TextArea txa = new TextArea("Choose a game mode.", 1, 50, TextArea.SCROLLBARS_VERTICAL_ONLY);
    static JPanel toolbar = new JPanel();
    static Image image[] = new Image[3];
    static Image sourse[] = new Image[3];
    static Random rand = new Random();

    static int game_start; // 1: vs player  2: vs computer
    static Color chess[] = new Color[2];
    static int ch; // who's turns
    static int board[][] = new int[13][13];
    static int result[] = new int[4]; // (x1, y1) (x2, y2)
    static int move[][] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
    static int current_theme = 0;
    static int bubbles[][] = new int[10][2];
    static boolean bbwin;

    public static void init()
    {       
        game_start = 0;
        bbwin = false;
        ch = 0;
        for (int i = 0; i < 13; ++i) for (int j = 0; j < 13; ++j) board[i][j] = 0;
    }

    public static void main(String args[])
    {
        init();
        // load image from file
        try { image[0] = ImageIO.read(new File("wood.png")); }
        catch(Exception ex) { System.out.println("No image"); }
        try { image[1] = ImageIO.read(new File("ground.jpg")); }
        catch(Exception ex) { System.out.println("No image"); }
        try { image[2] = ImageIO.read(new File("water.png")); }
        catch(Exception ex) { System.out.println("No image"); }
        try { sourse[1] = ImageIO.read(new File("a_rock.png")); }
        catch(Exception ex) { System.out.println("No image"); }
        try { sourse[2] = ImageIO.read(new File("bubble.png")); }
        catch(Exception ex) { System.out.println("No image"); }

        chess[0] = Color.white;
        chess[1] = Color.black;

        // set frm
        frm.addWindowListener(new WindowAdapter(){public void windowClosing(WindowEvent e){System.exit(0);}});
        frm.setTitle("Gobang");
        frm.setSize(800, 850);
        frm.setVisible(true);
        frm.setBackground(new Color(255,255,224));
        frm.addMouseListener(frm);
        frm.addMouseMotionListener(frm);
        
        // set button
        toolbar.setLayout(new FlowLayout());
        toolbar.add(txa);
        toolbar.add(player);
        toolbar.add(computer);
        toolbar.add(theme);
        toolbar.add(restart);
        player.addActionListener(frm);
        computer.addActionListener(frm);
        restart.addActionListener(frm);
        theme.addActionListener(frm);
        frm.add(toolbar, BorderLayout.SOUTH);
    }

    public boolean find_and_draw(int x, int y, int h)
    {
        Graphics2D g = (Graphics2D)getGraphics();
        int cur = 0;

        if (h == 2)
        {
            x = x * 50 + 80;
            y = y * 50 + 80;
        }

        // find the position of mouse and draw chess
        for (int i = 75, cnt_j = 0; i <= 675; i += 50, ++cnt_j)
            for (int j = 75, cnt_i = 0; j <= 675; j += 50, ++cnt_i) 
                if (x >= j && x <= j + 50 && y >= i && y <= i + 50 && board[cnt_j][cnt_i] == 0)
                {
                    cur = board[cnt_j][cnt_i] = ch + 1;

                    g.setColor(chess[ch]);
                    g.fillOval(j + 5, i + 5, 40, 40);

                    ch ^= 1;
                    g.setColor(chess[ch]);
                    g.drawOval(j + 5, i + 5, 40, 40);

                    // check if the game is end or not
                    if (isEnd(cnt_i, cnt_j, cur) == true)
                    {
                        if (bbwin == false)
                        {
                            g.setColor(Color.red);
                            g.setStroke(new BasicStroke(4));
                            g.drawLine(result[0] * 50 + 100, result[1] * 50 + 100, result[2] * 50 + 100, result[3] * 50 + 100);
                        } 

                        if (game_start == 1)
                        {
                            if (cur == 1) txa.setText("Player 1 wins!");
                            else txa.setText("Player 2 wins!");
                        }
                        else
                        {
                            if (cur == 1) txa.setText("Player wins!");
                            else txa.setText("Computer wins!");
                        }           

                        game_start = -1;
                    }
                    else
                    {
                        if (game_start == 1)
                        {
                            if (cur == 1) txa.setText("Player 1 goes (" + cnt_i + ", " + cnt_j  + ")  It's Player 2's turn.");
                            else txa.setText("Player 2 goes (" + cnt_i + ", " + cnt_j  + ")  It's Player 1's turn."); 
                        }
                        else
                        {
                            if (cur == 1) txa.setText("Player goes (" + cnt_i + ", " + cnt_j  + ")  It's Computer's turn.");
                            else txa.setText("Computer goes (" + cnt_i + ", " + cnt_j  + ")  It's Player's turn.");
                        }
                        
                    }

                    return true;
                } 

        return false;
    }

    public static boolean isEnd(int x, int y, int cur)
    {
        if (game_start == 2)
        {
            int cnt = 0;
            for (int i = 0; i < 10; ++i)
            {
                int bx = bubbles[i][0];
                int by = bubbles[i][1];

                if (board[by][bx] == cur) ++cnt;
            }
            if (cnt >= 7)
            {
                bbwin = true;
                return true;
            }
        }

        for (int i = 0; i < 4; ++i)
        {
            int cnt = 0;

            for (int j = -4; j <= 4; ++j)
            {
                int nx = x + move[i][0] * j;
                int ny = y + move[i][1] * j;

                if (nx >= 0 && nx < 13 && ny >= 0 && ny < 13 && board[ny][nx] == cur) ++cnt;
                else cnt = 0;

                if (cnt == 1)
                {
                    result[0] = nx;
                    result[1] = ny;
                }
                else if (cnt == 5) 
                { 
                    result[2] = nx;
                    result[3] = ny;
                    return true;
                }
            }
        }

        return false;
    }

    public void get_computer(int[] pos)
    {
        int best_attack[] = new int[2];
        int best_defence[] = new int[2];
        int tmp1[] = new int[2];
        int tmp2[] = new int[2];

        for (int i = 0; i < 13; ++i) for (int j = 0 ; j < 13; ++j)
        {
            if (board[i][j] != 0) continue;

            int c_ = score(j, i, 2);
            int p_ = score(j, i, 1);

            if (c_ > best_attack[0] || (c_ == best_attack[0] && p_ > best_attack[1]))
            {
                best_attack[0] = c_;
                best_attack[1] = p_;
                tmp1[0] = j;
                tmp1[1] = i;
            }
            if (p_ > best_defence[0] || (p_ == best_defence[0] && c_ > best_defence[1]))
            {
                best_defence[0] = p_;
                best_defence[1] = c_;
                tmp2[0] = j;
                tmp2[1] = i;
            }
        }
        //System.out.println(best_attack[0] + " " + best_attack[1] + " " + best_defence[0] + " " + best_defence[1]);

        if (best_attack[0] > best_defence[0])
        {
            pos[0] = tmp1[0];
            pos[1] = tmp1[1];
        }
        else if (best_defence[0] > best_attack[0])
        {
            pos[0] = tmp2[0];
            pos[1] = tmp2[1];
        }
        else if (best_attack[1] >= best_defence[1])
        {
            pos[0] = tmp1[0];
            pos[1] = tmp1[1];
        }
        else
        {
            pos[0] = tmp2[0];
            pos[1] = tmp2[1];
        }
    }

    public int score(int x, int y, int cur)
    {
        int opposite = (cur == 1 ? 2 : 1);
        int five = 0;
        int four_alive = 0, four_die1 = 0, four_die2 = 0;
        int three_alive1 = 0, three_alive2 = 0, three_die = 0;
        int two_alive = 0, two_die = 0;

        for (int i = 0; i < 4; ++i)
        {
            int cnt = 1;
            int l = 0, r = 0;
            int left[] = new int[4];
            int right[] = new int[4];

            // find right
            for (int j = 1; j <= 4; ++j)
            {
                int nx = x + move[i][0] * j;
                int ny = y + move[i][1] * j;

                if (nx >= 0 && nx < 13 && ny >= 0 && ny < 13 && board[ny][nx] == cur) ++cnt;
                else 
                {
                    r = j;
                    break;
                }
            }

            // find left
            for (int j = -1; j >= -4; --j)
            {
                int nx = x + move[i][0] * j;
                int ny = y + move[i][1] * j;

                if (nx >= 0 && nx < 13 && ny >= 0 && ny < 13 && board[ny][nx] == cur) ++cnt;
                else
                {
                    l = j;
                    break;
                }
            }

            // get left and right array
            for (int j = 0; j < 4; ++j, --l, ++r)
            {
                int nx = x + move[i][0] * l;
                int ny = y + move[i][1] * l;
                if (nx >= 0 && nx < 13 && ny >= 0 && ny < 13) left[j] = board[ny][nx];
                else left[j] = opposite;

                nx = x + move[i][0] * r;
                ny = y + move[i][1] * r;
                if (nx >= 0 && nx < 13 && ny >= 0 && ny < 13) right[j] = board[ny][nx];
                else right[j] = opposite;
            }

            if (cnt == 5) ++five;
            else if (cnt == 4)
            {
                if (left[0] == 0 && right[0] == 0) ++four_alive; // _oooo_
                else if (left[0] == 0 || right[0] == 0) ++four_die1; // _oooo, oooo_
            }
            else if (cnt == 3)
            {
                if ((left[0] == 0 && left[1] == cur) || (right[0] == 0 && right[1] == cur)) ++four_die2; // o_ooo, ooo_o
                else if  (left[0] == 0 && right[0] == 0 && (left[1] == 0 || right[1] == 0)) ++three_alive1; // __ooo_, _ooo__
                else if ((left[0] == 0 && left[1] == 0) || (right[0] == 0 && right[1] == 0)) ++three_die; // __ooo, ooo__
                else if (left[0] == 0 && right[0] == 0) ++three_die; // _ooo_
            }
            else if (cnt == 2)
            {
                if ((left[0] == 0 && left[1] == cur && left[2] == cur) || (right[0] == 0 && right[1] == cur && right[2] == cur)) ++four_die2; // oo_oo
                else if ((left[0] == 0 && right[0] == 0) && (left[1] == cur && left[2] == 0) || (right[1] == cur && right[2] == 0)) ++three_alive2; // _o_oo_, _oo_o_
                else if ((left[0] == 0 && left[1] == cur && left[2] == 0) || (right[0] == 0 && right[1] == cur && right[2] == 0)) ++three_die; //_o_oo, oo_o_
                else if ((left[0] == 0 && left[1] == 0 && left[2] == cur) || (right[0] == 0 && right[1] == 0 && right[2] == cur)) ++three_die; // o__oo, oo__o
                else if (left[0] == 0 && right[0] == 0 && (left[1] == 0 && left[2] == 0) || (left[1] == 0 && right[1] == 0) || (right[1] == 0 && right[2] == 0)) ++two_alive; // _oo___, __oo__, ___oo_
                else if ((left[0] == 0 && left[1] == 0 && left[2] == 0) && (right[0] == 0 && right[1] == cur && right[2] == 0)) ++two_die; // ___oo, oo___
            }
            else if (cnt == 1)
            {
                if ((left[0] == 0 && left[1] == cur && left[2] == cur && left[3] == cur) || (right[0] == 0 && right[1] == cur && right[2] == cur && right[3] == cur)) ++four_die2; // ooo_o, o_ooo
                else if ((left[0] == 0 && right[0] == 0) && ((left[1] == cur && left[2] == cur && left[3] == 0) || ( right[1] == cur && right[2] == cur && right[3] == 0))) ++three_alive2; // _oo_o_, _o_oo_
                else if (left[0] == 0 && right[0] == 0 && ((left[1] == cur && left[2] == cur) || (right[1] == cur && right[2] == cur))) ++three_die; // oo_o_, _o_oo
                else if ((left[0] == 0 && left[1] == cur && left[2] == cur && left[3] == 0) || (right[0] == 0 && right[1] == cur && right[2] == cur && right[3] == 0)) ++three_die; // _oo_o, o_oo_
                else if ((left[0] == 0 && left[1] == 0 && left[2] == cur && left[3] == cur) || (right[0] == 0 && right[1] == 0 && right[2] == cur && right[3] == cur)) ++three_die; // oo__o, o__oo
                else if ((left[0] == 0 && left[1] == cur && left[2] == 0 && left[3] == cur) || (right[0] == 0 && right[1] == cur && right[2] == 0 && right[3] == cur)) ++three_die; // o_o_o
            }
        }

        if (five >= 1) return 100; // ooooo
        else if (four_alive >= 1 || (four_die1 + four_die2) >= 2 || ((four_die1 + four_die2) >= 1 && (three_alive1 + three_alive2) >= 1)) return 13;
        else if ((three_alive1 + three_alive2)  >= 2) return 12;
        else if (three_alive1 >= 1 && (four_die1 + four_die2) >= 1) return 11;
        else if (three_alive2 >= 1 && (four_die1 + four_die2) >= 1) return 10;
        else if (three_alive1 >= 1) return 9;
        else if (three_alive2 >= 1) return 8;
        else if (four_die1 >= 1) return 7;
        else if (four_die2 >= 1) return 6;
        else if (two_alive >= 2) return 5;
        else if (two_alive >= 1) return 4;
        else if (three_die >= 1) return 3;
        else if (two_die >= 1) return 2;
        else return 1;
    }

    public void draw_item()
    {
        Graphics2D g = (Graphics2D)getGraphics();
        int cnt = 0, x, y;
              
        do
        {
            x = rand.nextInt(13);
            y = rand.nextInt(13);
            if (board[y][x] != 0) continue;

            g.drawImage(sourse[current_theme], 70 + x * 50, 70 + y * 50, 60, 60, null);
            if (current_theme == 1) board[y][x] = 3;
            else if (current_theme == 2)
            {
                bubbles[cnt][0] = x;
                bubbles[cnt][1] = y;
            }
            ++cnt;

        } while (cnt != 10);
    }

    public void paint(Graphics g)
	{
        Graphics2D g2 = (Graphics2D)g;
      
        g2.drawImage(image[current_theme], 50, 50, 700, 700, null);

        // draw the chessboard
        g2.setStroke(new BasicStroke(2));
        if (current_theme == 0) g2.setColor(Color.black);
        else if (current_theme == 1) g2.setColor(Color.white);

        for (int i = 100; i <= 700; i += 50) 
        { 
            g2.drawLine(i, 100, i, 700);
            g2.drawLine(100, i, 700, i);
        }
        g2.fillOval(245, 245, 10, 10);
        g2.fillOval(545, 545, 10, 10);
        g2.fillOval(245, 545, 10, 10);
        g2.fillOval(545, 245, 10, 10);
        g2.fillOval(395, 395, 10, 10);
	}

    public void actionPerformed(ActionEvent e)
    {
        JButton b = (JButton) e.getSource();

        if (game_start == 0 && b == player)
        {
            txa.setText("Player vs player. Game started! White turn.");
            game_start = 1;
            if (current_theme >= 1) draw_item();
        }
        else if (game_start == 0 && b == computer)
        {
            txa.setText("Player vs computer. Game started! White turn.");
            game_start = 2;
            if (current_theme >= 1) draw_item();
        }
        else if (b == restart)
        {
            txa.setText("Game restart. Choose a game mode.");
            init();
            repaint();
        }
        else if (game_start == 0 && b == theme)
        {
            ++current_theme;

            if (current_theme == 3) current_theme = 0;
            if (current_theme == 0)
            {
                chess[0] = Color.white;
                chess[1] = Color.black;
            }
            else if (current_theme == 1)
            {
                chess[0] = Color.red;
                chess[1] = Color.blue;
            }
            else if (current_theme == 2)
            {
                chess[0] = Color.green;
                chess[1] = Color.yellow;
            }

            repaint();
        }
    }

    public void mouseClicked(MouseEvent e)
    {
        if (game_start <= 0) return;

        if (find_and_draw(e.getX(), e.getY(), 1) && game_start == 2)
        {
            int pos[] = new int[2];
            get_computer(pos);
            find_and_draw(pos[0], pos[1], 2);
        } 
    }
    public void mouseMoved(MouseEvent e){}
	public void mouseReleased(MouseEvent e){}
	public void mouseEntered(MouseEvent e){}
	public void mouseExited(MouseEvent e){}
	public void mouseDragged(MouseEvent e){}
	public void mousePressed(MouseEvent e){}
}