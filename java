JSP


<%@ page import="java.sql.*" %>
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Student Details</title>
</head>
<body>

<%..
    String operation = request.getParameter("operation");
    String usn = request.getParameter("usn");

    // Database connection details
    String jdbcUrl = "jdbc:mysql://localhost:3306/1ms21cs016";
    String dbUser = "root";
    String dbPassword = "";

    try {
        // Load JDBC driver
        Class.forName("com.mysql.jdbc.Driver");

        // Establish database connection
        Connection connection = DriverManager.getConnection(jdbcUrl, dbUser, dbPassword);

        switch (operation) {
            case "insert":
                // Insert operation
                String insertSql = "INSERT INTO stud (usn, name) VALUES (?, ?)";
                PreparedStatement insertStatement = connection.prepareStatement(insertSql);
                insertStatement.setString(1, usn);
                insertStatement.setString(2, request.getParameter("name")); // Assuming a parameter named "name"
                insertStatement.executeUpdate();
                out.println("<h2>Data inserted successfully</h2>");
                insertStatement.close();
                break;

            case "update":
                // Update operation
                String updateSql = "UPDATE stud SET name=? WHERE usn=?";
                PreparedStatement updateStatement = connection.prepareStatement(updateSql);
                updateStatement.setString(1, request.getParameter("name")); // Assuming a parameter named "name"
                updateStatement.setString(2, usn);
                updateStatement.executeUpdate();
                out.println("<h2>Data updated successfully</h2>");
                updateStatement.close();
                break;

            case "delete":
                // Delete operation
                String deleteSql = "DELETE FROM stud WHERE usn=?";
                PreparedStatement deleteStatement = connection.prepareStatement(deleteSql);
                deleteStatement.setString(1, usn);
                deleteStatement.executeUpdate();
                out.println("<h2>Data deleted successfully</h2>");
                deleteStatement.close();
                break;

            case "extract":
                // Extract operation
                String selectSql = "SELECT * FROM stud WHERE usn=?";
                PreparedStatement selectStatement = connection.prepareStatement(selectSql);
                selectStatement.setString(1, usn);
                ResultSet resultSet = selectStatement.executeQuery();

                if (resultSet.next()) {
%>
                    <h2>Student Details</h2>
                    <p><strong>USN:</strong> <%= resultSet.getString("usn") %></p>
                    <p><strong>Name:</strong> <%= resultSet.getString("name") %></p>
<%
                } else {
%>
                    <p style="color: red;">Invalid USN. Student not found.</p>
<%
                }

                // Close resources
                resultSet.close();
                selectStatement.close();
                break;

            default:
                out.println("<h2>Invalid operation</h2>");
        }

        // Close connection
        connection.close();

    } catch (ClassNotFoundException | SQLException e) {
        e.printStackTrace();
    }
%>

</body>
</html>


html

HTML FILE 

<!DOCTYPE html>
<html>
<head>
    <title>Student Database Operations</title>
</head>
<body>

<h2>Insert/Update Student</h2>
<form action="NewFile.jsp" method="post">
    <input type="hidden" name="operation" value="insert">
    USN: <input type="text" name="usn" required>
    Name: <input type="text" name="name" required>
    <input type="submit" value="Insert">
</form>

<br>

<h2>Delete Student</h2>
<form action="NewFile.jsp" method="post">
    <input type="hidden" name="operation" value="delete">
    USN: <input type="text" name="usn" required>
    <input type="submit" value="Delete">
</form>

<br>

<h2>Extract Student Details</h2>
<form action="NewFile.jsp" method="post">
    <input type="hidden" name="operation" value="extract">
    USN: <input type="text" name="usn" required>
    <input type="submit" value="Extract">
</form>

<br>

<h2>Update Student Name</h2>
<form action="NewFile.jsp" method="post">
    <input type="hidden" name="operation" value="update">
    USN: <input type="text" name="usn" required>
    New Name: <input type="text" name="name" required>
    <input type="submit" value="Update">
</form>

</body>
</html>
