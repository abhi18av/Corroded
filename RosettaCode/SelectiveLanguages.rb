all_my_languages = [ "AWK",
"Brainf---",
"C ",
"C-sharp",
"C++",
"Clojure",
"Crystal",
"CoffeeScript",
"Common-Lisp",
"Elixir",
"Emacs-Lisp",
"F-Sharp",
"Go",
"Haskell",
"Haxe",
"Idris",
"Java",
"JavaScript",
"Julia",
"Lua",
"LLVM",
"Mathematica",
"MATLAB",
"Octave",
"OCaml",
"Perl",
"PowerShell",
"Python",
"R",
"Ruby",
"Rust",
"Sage",
"Scala",
"Scheme",
"Standard-ML",
"Swift",
"TypeScript",
"UNIX-Shell",
"UnixPipes",
]


my_languages = ["Rust", "C"]

#puts my_languages

all_files = Dir['./resources/Task/*']

# to keep only folders 
folders = all_files.reject {|f| File.file? f}





for fldr in folders
# also move and rename the descriptions file
lang_nameS = []
#move inside every folder

cmd1 = "mkdir " + "./SelectiveLanguages/"  + fldr.split("/")[-1]
#puts cmd1

	langS = Dir[fldr + "/*"]
	
	#puts langs
	lang_nameS.push(langS)

	

#for  lang in lang_nameS

#		lang_name = lang.split("/")[-1]
#		puts lang_name
=begin
		if my_languages.include? lang
			puts lang
		x = lang.split("/")[-1]

		puts x
=end
	#remove lang folder

		#end



	#end	
end

