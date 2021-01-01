using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace _1083321_hw8
{
    public partial class Form1 : Form
    {
        double s;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            // TODO: 這行程式碼會將資料載入 'dbDataSet1.資料表1' 資料表。您可以視需要進行移動或移除。
            this.資料表1TableAdapter1.Fill(this.dbDataSet1.資料表1);
            // TODO: 這行程式碼會將資料載入 'dbDataSet.資料表1' 資料表。您可以視需要進行移動或移除。
            this.資料表1TableAdapter1.Fill(this.dbDataSet1.資料表1);
            s = (double.Parse(textBox4.Text) + double.Parse(textBox5.Text)) / 2;
            if (textBox4.Text != "" && textBox5.Text != "")
                label7.Text = (s).ToString();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if ((this.資料表1BindingSource1.Find("StudentID", textBox1.Text)) != -1)
            {
                MessageBox.Show("This id exists!");
            }
            else
            {
                this.資料表1TableAdapter1.Insert(textBox1.Text, textBox2.Text, textBox3.Text, textBox4.Text, textBox5.Text);
                this.資料表1TableAdapter1.Fill(this.dbDataSet1.資料表1);
            }

        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (double.Parse(textBox4.Text) < 0 || double.Parse(textBox4.Text) > 100 || double.Parse(textBox5.Text) < 0 || double.Parse(textBox5.Text) > 100)
            {
                MessageBox.Show("Error!");
            }
            else
            {
                this.資料表1BindingSource1.EndEdit();
                this.資料表1TableAdapter1.Update(this.dbDataSet1);
            }
            
        }

        private void button3_Click(object sender, EventArgs e)
        {
            this.資料表1TableAdapter1.Delete(textBox1.Text, textBox2.Text, textBox3.Text, textBox4.Text, textBox5.Text);
            this.資料表1TableAdapter1.Fill(this.dbDataSet1.資料表1);

        }

        private void button4_Click(object sender, EventArgs e)
        {
            int i = -1;
            switch (comboBox1.Text)
            {
                case "ID":
                    i = this.資料表1BindingSource1.Find("StudentID", textBox6.Text);
                    break;
                case "Name":
                    i = this.資料表1BindingSource1.Find("StudentName", textBox6.Text);
                    break;
                case "Gender":
                    i = this.資料表1BindingSource1.Find("Gender", textBox6.Text);
                    break;
                case "Mid":
                    i = this.資料表1BindingSource1.Find("MidExam", textBox6.Text);
                    break;
                case "Final":
                    i = this.資料表1BindingSource1.Find("FinalExam", textBox6.Text);
                    break;
            }
            if (i != -1)
                this.資料表1BindingSource1.Position = i;
            else
                MessageBox.Show("Not found!");

        }

        private void textBox4_TextChanged(object sender, EventArgs e)
        {
            if (textBox4.Text != "" && textBox5.Text != "")
            {
                s = (double.Parse(textBox4.Text) + double.Parse(textBox5.Text)) / 2;
                label7.Text = (s).ToString();
                Invalidate();
            }
        }

        private void textBox5_TextChanged(object sender, EventArgs e)
        {
            if (textBox4.Text != "" && textBox5.Text != "")
            {
                s = (double.Parse(textBox4.Text) + double.Parse(textBox5.Text)) / 2;
                label7.Text = (s).ToString();
                Invalidate();
            }
        }

        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            for (int a = 60; a <= s; a += 10)
            {
                e.Graphics.DrawImage(Properties.Resources._8_1, 150 + (a-60)*3, 300, 30, 30);
            }
            
        }
    }
}
