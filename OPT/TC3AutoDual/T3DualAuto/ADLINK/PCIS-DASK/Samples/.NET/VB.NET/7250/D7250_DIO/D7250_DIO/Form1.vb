Public Class Form1
    Dim m_dev As Short

    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        m_dev = Register_Card(PCI_7250, 0)
        If (m_dev < 0) Then
            MessageBox.Show("Register_Card error!")
        End If
    End Sub

    Private Sub Form1_FormClosing(ByVal sender As System.Object, ByVal e As System.Windows.Forms.FormClosingEventArgs) Handles MyBase.FormClosing
        Dim err As Short
        If (m_dev >= 0) Then
            err = DASK.Release_Card(m_dev)
        End If
    End Sub

    Private Sub button1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles button1.Click
        Dim err As Short
        Dim out_value As Integer
        If (Int32.TryParse(textBox1.Text, out_value)) Then
            err = DO_WritePort(m_dev, 0, out_value)
            If (err < 0) Then
                MessageBox.Show("DO_WritePort error!")
            End If
        Else
            MessageBox.Show("Input error!")
        End If
    End Sub

    Private Sub button2_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles button2.Click
        Dim err As Short
        Dim int_value As UInteger
        err = DASK.DI_ReadPort(m_dev, 0, int_value)
        If (err < 0) Then
            MessageBox.Show("D2K_DI_ReadPort error!")
            Return
        End If
        textBox2.Text = int_value
    End Sub
End Class
