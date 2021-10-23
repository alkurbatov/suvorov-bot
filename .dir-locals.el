((c++-mode
  (flycheck-cppcheck-standards      . "c++14")
  (flycheck-clang-language-standard . "c++14")
  (flycheck-gcc-language-standard   . "c++14")
  (eval progn (let
                  ((local-include-paths
                    (list (concat (projectile-project-root) "build")
                          (concat (projectile-project-root) "src")
                          (concat (projectile-project-root) "contrib/cpp-sc2/include")
                          (concat (projectile-project-root) "contrib/cpp-sc2/contrib/protobuf/src")
                          (concat (projectile-project-root) "contrib/cpp-sc2/generated")
                          (concat (projectile-project-root) "build/contrib/cpp-sc2/generated"))))
                (setq flycheck-clang-include-path local-include-paths)
                (setq flycheck-gcc-include-path local-include-paths)))))
