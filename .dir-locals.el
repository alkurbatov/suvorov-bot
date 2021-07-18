;;; Directory Local Variables
;;; For more information see (info "(emacs) Directory Variables")

((nil
  (eval . (let ((root (projectile-project-root)))
            (setq-local flycheck-cppcheck-standards "c++14")
            (setq-local flycheck-clang-language-standard "c++14")
            (setq-local flycheck-clang-include-path
                        (list (concat root "src")
                              (concat root "contrib/cpp-sc2/include")
                              (concat root "contrib/cpp-sc2/contrib/protobuf/src")
                              (concat root "contrib/cpp-sc2/generated")
                              (concat root "build/contrib/cpp-sc2/generated")))))))
