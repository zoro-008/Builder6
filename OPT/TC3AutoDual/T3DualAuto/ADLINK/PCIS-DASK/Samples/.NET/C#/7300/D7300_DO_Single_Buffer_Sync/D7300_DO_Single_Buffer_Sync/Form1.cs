using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace D7300_DO_Single_Buffer_Sync
{
    public partial class Form1 : Form
    {
        ushort[] data_buffer = new ushort[1000];
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
            int i;
            short err;
            for (i = 0; i < 1000; i++)
            {
                data_buffer[i] = (ushort)i;
            }
            err = DASK.DO_7300B_Config((ushort)m_dev, 16, DASK.TRIG_INT_PACER, DASK.P7300_WAIT_NO, DASK.P7300_TERM_OFF, 0, 0x29810);
            if (err != 0)
            {
                MessageBox.Show("DO_7300B_Config error!");
                return;
            }
            err = DASK.DO_ContWritePort((ushort)m_dev, 0, data_buffer, 1000,10, 10000, DASK.SYNCH_OP);
            if (err != 0)
            {
                MessageBox.Show("DO_ContWritePort error!");
                return;
            }
        }
    }
}