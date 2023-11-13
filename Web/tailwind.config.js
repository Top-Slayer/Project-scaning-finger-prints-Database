/** @type {import('tailwindcss').Config} */
module.exports = {
  content: ["./**/*.{html,js}"],
  theme: {
    extend: {
      colors: {
        "yellowdark": "#FAC402",
        "graywhite": "#F4F1F1" ,
        "GreenWhite": "#03B54B" ,
        "ReadBold":"#CE2319",
        "BlueWhite": "#192655",
        "BlueGray": "#495789"
      },
      spacing: {
        '47': '185px',
      },
      // padding:{
      //   "15":"58px",
      //   "13":"51px",
        
      // }
    },
  },
  plugins: [],
}

