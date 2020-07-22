((c++-mode . ((cmake-ide-build-dir . "build"))))

(
 (nil . (
	 (eval .
	       (setq flycheck-clang-include-path
                     (list (expand-file-name "/usr/local/src/CPP-helloworld/openPMD-api/include/")
			   
			   )
		     )
	       (setq flycheck-gcc-include-path
                     (list (expand-file-name "/usr/local/src/CPP-helloworld/openPMD-api/include/")
			   )
		     )
	       )
	 )
      )
 )
