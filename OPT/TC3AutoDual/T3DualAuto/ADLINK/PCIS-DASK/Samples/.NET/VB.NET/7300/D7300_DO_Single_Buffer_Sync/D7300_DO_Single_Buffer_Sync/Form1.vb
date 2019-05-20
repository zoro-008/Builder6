Public Class Form1
    Dim m_dev As Short
    Dim data_buffer(999) As UShort

    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        m_dev = Register_Card(PCI_7300A_RevB, 0)
        If (m_dev < 0) Then
            MessageBox.Show("DO_7300B_Config error!")
        End If
    End Sub

    Private Sub Form1_FormClosing(ByVal sender As Object, ByVal e As System.Windows.Forms.FormClosingEventArgs) Handles Me.FormClosing
        Dim ret As Short
        If (m_dev >= 0) Then
            ret = DASK.Release_Card(m_dev)
        End If
    End Sub

    Private Sub Button1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button1.Click
        Dim i As Integer
        Dim err As Short
        Dim port As Short

        For i = 0 To 999
            data_buffer(i) = i
        Next

        err = DO_7300B_Config(m_dev, 16, DASK.TRIG_INT_PACER, DASK.P7300_WAIT_NO, DASK.P7300_TERM_OFF, 0, &H29810)
        If (err <> 0) Then
            MessageBox.Show("DO_7300B_Config error!")
            Return
        End If
        port = 0
        err = DO_ContWritePort(m_dev, 0, data_buffer(0), 1000, 10, 10000, SYNCH_OP)
        If (err <> 0) Then
            MessageBox.Show("DO_ContWritePort error!")
            Return
        End If
    End Sub
End Class
