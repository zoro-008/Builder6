Public Class Form1
    Dim m_dev As Short
    Dim data_buffer(99) As UShort

    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        m_dev = Register_Card(PCI_7300A_RevB, 0)
        If (m_dev < 0) Then
            MessageBox.Show("DO_7300B_Config error!")
        End If
    End Sub

    Private Sub Form1_FormClosing(ByVal sender As System.Object, ByVal e As System.Windows.Forms.FormClosingEventArgs) Handles MyBase.FormClosing
        Dim ret As Short
        If (m_dev >= 0) Then
            ret = DASK.Release_Card(m_dev)
        End If
    End Sub

    Private Sub button1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles button1.Click
        Dim i As Integer
        Dim err As Short
        err = DASK.DI_7300B_Config(m_dev, 16, TRIG_INT_PACER, P7300_WAIT_NO, P7300_TERM_OFF, P7300_DIREQ_POS Or P7300_DIACK_POS Or P7300_DITRIG_POS, False, False)
        If (err <> 0) Then
            MessageBox.Show("DI_7300B_Config error!")
            Return
        End If
        err = DI_ContReadPort(m_dev, 0, data_buffer(0), 100, 10000, SYNCH_OP)
        If (err <> 0) Then
            MessageBox.Show("DI_ContReadPort error!")
            Return
        End If
        listBox1.Items.Clear()
        For i = 0 To 99
            listBox1.Items.Add(data_buffer(i))
        Next
    End Sub
End Class
