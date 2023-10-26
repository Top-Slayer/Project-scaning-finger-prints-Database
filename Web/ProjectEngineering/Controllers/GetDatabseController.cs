using Microsoft.AspNetCore.Mvc;
using MySql.Data.MySqlClient;

namespace ProjectEngineering.Controllers
{
    public class GetDatabaseController : Controller
    {
        public IActionResult Index()
        {
            return Content("Scores of computer");
        }

        public IActionResult Detail()
        {
            return Content("Detail of scores");
        }
    }
}