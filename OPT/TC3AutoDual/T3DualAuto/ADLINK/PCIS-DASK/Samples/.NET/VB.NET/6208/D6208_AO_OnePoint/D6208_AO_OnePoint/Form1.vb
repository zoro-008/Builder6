Public Class Form1
    Dim m_dev As Short

    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        m_dev = Register_Card(PCI_6208V, 0)
        If (m_dev < 0) Then
            MessageBox.Show("PCI_6208V error!")
        End If
    End Sub

    Private Sub Form1_FormClosing(ByVal sender As System.Object, ByVal e As System.Windows.Forms.FormClosingEventArgs) Handles MyBase.FormClosing
        Dim ret As Short
        If (m_dev >= 0) Then
            ret = DASK.Release_Card(m_dev)
        End If
    End Sub

    Private Sub button1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles button1.Click
        Dim err As Short
        Dim out_value As Double
        If (Double.TryParse(textBox1.Text, out_value)) Then
            err = DASK.AO_VWriteChannel(m_dev, 0, out_value)
            If (err < 0) Then
                MessageBox.Show("AO_VWriteChannel error!")
            End If
        Else
            MessageBox.Show("Input error!")
        End If
    End Sub
End Class
