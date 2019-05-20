using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace D9112_AI_Single_Buffer_Check
{
    public partial class Form1 : Form
    {
        ushort[] data_buffer = new ushort[1000];
        double[] voltage_array = new double[1000];
        public short m_dev;
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            m_dev = DASK.Register_Card(DASK.PCI_9112, 0);
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
            byte stopped;
            uint access_cnt;
            ret = DASK.AI_9112_Config((ushort)m_dev, DASK.TRIG_INT_PACER);
            if (ret < 0)
            {
                MessageBox.Show("AI_9112_Config error!");
                return;
            }
            ret = DASK.AI_ContReadChannel((ushort)m_dev, 0, DASK.AD_B_5_V, data_buffer, 1000, 10000, DASK.ASYNCH_OP);
            if (ret < 0)
            {
                MessageBox.Show("AI_ContReadChannel error!");
                return;
            }
            do
            {
                ret = DASK.AI_AsyncCheck((ushort)m_dev, out stopped, out access_cnt);
                if (ret < 0)
                {
                    MessageBox.Show("AI_AsyncCheck error!");
                    return;
                }
            } while (stopped == 0);
            ret = DASK.AI_ContVScale((ushort)m_dev, DASK.AD_B_5_V, data_buffer, voltage_array, 1000);
            if (ret < 0)
            {
                MessageBox.Show("AI_ContVScale error!");
                return;
            }
            axDGraph1.PlotGraph(voltage_array, 0);
        }
    }
}