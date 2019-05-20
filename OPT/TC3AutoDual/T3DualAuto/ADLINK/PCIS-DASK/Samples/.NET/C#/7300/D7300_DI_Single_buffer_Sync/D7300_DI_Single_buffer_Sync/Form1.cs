using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace D7300_DI_Single_buffer_Sync
{
    public partial class Form1 : Form
    {
        ushort[] data_buffer = new ushort[100];
        public short m_dev;
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            m_dev = DASK.Register_Card(DASK.PCI_7300A_RevB, 0);
            if (m_dev < 0)
            {
                MessageBox.Show("Register_Card error!");

            }
        }

        private void Form1_InputLanguageChanged(object sender, InputLanguageChangedEventArgs e)
        {
            short ret;
            if (m_dev >= 0)
            {
                ret = DASK.Release_Card((ushort)m_dev);
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            int i;
            short err;
            err = DASK.DI_7300B_Config((ushort)m_dev, 16, DASK.TRIG_INT_PACER, DASK.P7300_WAIT_NO, DASK.P7300_TERM_OFF, DASK.P7300_DIREQ_POS | DASK.P7300_DIACK_POS | DASK.P7300_DITRIG_POS, false, false);
            if (err != 0)
            {
                MessageBox.Show("DI_7300B_Config error!");
                return;
            }
            err = DASK.DI_ContReadPort((ushort)m_dev, 0, data_buffer, 100, 10000, DASK.SYNCH_OP);
            if (err != 0)
            {
                MessageBox.Show("DI_ContReadPort error!");
                return;
            }
            listBox1.Items.Clear();
            for (i = 0; i < 100; i++)
            {
                listBox1.Items.Add(data_buffer[i]);
            }
        }
    }
}