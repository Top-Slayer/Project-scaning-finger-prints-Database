using System.Diagnostics;
using Microsoft.AspNetCore.Mvc;
using ProjectEngineering.Models;
using MySql.Data.MySqlClient;
using MySqlX.XDevAPI.Relational;

namespace ProjectEngineering.Controllers;

public class HomeController : Controller
{

    private readonly ILogger<HomeController> _logger;

    public HomeController(ILogger<HomeController> logger)
    {
        _logger = logger;
    }

    public IActionResult Index()
    {
        MySqlConnection connection = new MySqlConnection("Database=finger_prints;Data Source=127.0.0.1;User Id=root;Password=topslayers12#");
        connection.Open();

        MySqlCommand command = connection.CreateCommand();
        command.CommandText = "select * from finger_apperances";

        List<string> nameValue = new List<string>();
        List<string> loginDateValue = new List<string>();
        List<string> accessDateValue = new List<string>();
        List<string> leaveDateValue = new List<string>();

        MySqlDataReader reader = command.ExecuteReader();

        while (reader.Read())
        {
            nameValue.Add(reader.GetString(1));
            loginDateValue.Add(reader.GetString(2));
        }

        try
        {
            string[] varName = { "name1", "name2", "name3", "name4", "name5", "name6", };
            string[] varLogin = { "login1", "login2", "login3", "login4", "login5", "login6", };

            for (int i = 0; i <= 5; i++)
            {
                ViewData[varName[i]] = nameValue[i];
                ViewData[varLogin[i]] = loginDateValue[i];
            }
        }
        catch (System.ArgumentOutOfRangeException){}

        return View();
    }

    public IActionResult Privacy()
    {
        return View();
    }

    [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
    public IActionResult Error()
    {
        return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
    }
}
