using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace D6208_AO_OnePoint
{
    public partial class Form1 : Form
    {
        public short m_dev;
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            m_dev = DASK.Register_Card(DASK.PCI_6208V, 0);
            if (m_dev < 0)
            {
                MessageBox.Show("Register_Card error!");
            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            short ret;
            if (m_dev >= 0)
            {
                ret = DASK.Release_Card((ushort)m_dev);
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            short ret;
            double out_value;
            if (Double.TryParse(textBox1.Text, out out_value))
            {
                ret = DASK.AO_VWriteChannel((ushort)m_dev, 0, out_value);
                if (ret < 0)
                {
                    MessageBox.Show("AO_VWriteChannel error!");
                }
            }
            else
            {
                MessageBox.Show("Input error!");
            }
        }
    }
}