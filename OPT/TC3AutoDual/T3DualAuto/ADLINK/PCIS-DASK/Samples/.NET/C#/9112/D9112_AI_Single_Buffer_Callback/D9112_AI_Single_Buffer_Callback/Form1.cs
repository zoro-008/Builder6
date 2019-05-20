using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace D9112_AI_Single_Buffer_Callback
{
    public partial class Form1 : Form
    {
        public short m_dev;
        CallbackDelegate m_del;
        ushort[] m_data_buffer = new ushort[1000];
        double[] m_voltage_array = new double[1000];
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            m_del = new CallbackDelegate(Callback);
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
            ret = DASK.AI_EventCallBack((ushort)m_dev, 1, DASK.AIEnd, m_del);
            if (ret < 0)
            {
                MessageBox.Show("D2K_AI_EventCallBack error!");
                return;
            }
            ret = DASK.AI_9112_Config((ushort)m_dev, DASK.TRIG_INT_PACER);
            if (ret < 0)
            {
                MessageBox.Show("AI_9112_Config error!");
                return;
            }
            ret = DASK.AI_ContReadChannel((ushort)m_dev, 0, DASK.AD_B_5_V, m_data_buffer, 1000, 10000, DASK.ASYNCH_OP);
            if (ret < 0)
            {
                MessageBox.Show("AI_ContReadChannel error!");
                return;
            }
        }
        private void Callback()
        {
            short ret;
            ret = DASK.AI_ContVScale((ushort)m_dev, DASK.AD_B_5_V, m_data_buffer, m_voltage_array, 1000);
            if (ret < 0)
            {
                MessageBox.Show("D2K_AI_ContVScale error!");
                return;
            }
            axDGraph1.PlotGraph(m_voltage_array, 0);
        }
    }
}