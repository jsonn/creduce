#ifndef REPLACE_CALL_EXPR_H
#define REPLACE_CALL_EXPR_H

#include <string>
#include <vector>
#include <utility>
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/DenseMap.h"
#include "Transformation.h"

namespace clang {
  class DeclGroupRef;
  class ASTContext;
  class FunctionDecl;
  class CallExpr;
  class DeclRefExpr;
  class ReturnStmt;
  class Expr;
  class ParmVarDecl;
}

class ReplaceCallExprVisitor;

class ReplaceCallExpr : public Transformation {
friend class ReplaceCallExprVisitor;

public:

  ReplaceCallExpr(const char *TransName, const char *Desc)
    : Transformation(TransName, Desc),
      CollectionVisitor(NULL),
      TheCallExpr(NULL),
      TheReturnStmt(NULL),
      CurrentFD(NULL)
  { }

  ~ReplaceCallExpr(void);

private:
  
  typedef llvm::SmallVector<clang::ReturnStmt *, 5> ReturnStmtsVector;

  typedef llvm::SmallVector<const clang::DeclRefExpr *, 5> ParmRefsVector;

  virtual void Initialize(clang::ASTContext &context);

  virtual void HandleTopLevelDecl(clang::DeclGroupRef D);

  virtual void HandleTranslationUnit(clang::ASTContext &Ctx);

  void addOneReturnStmt(clang::ReturnStmt *RS);

  void addOneParmRef(clang::ReturnStmt *RS, const clang::DeclRefExpr *DE);

  void replaceCallExpr(void);

  void getNewParmRefStr(const clang::DeclRefExpr *DE, 
                        std::string &ParmRefStr);

  void replaceParmRefs(std::string &RetStr, const clang::Expr *RetE,
    llvm::DenseMap<const clang::DeclRefExpr *, std::string> &ParmRefToStrMap);

  void insertParmRef(
    std::vector< std::pair<const clang::DeclRefExpr *, int> > &SortedParmRefs,
    const clang::DeclRefExpr *ParmRef, int Off);

  void sortParmRefsByOffs(const char *StartBuf, 
     llvm::DenseMap<const clang::DeclRefExpr *, std::string> &ParmRefToStrMap,
     std::vector<std::pair<const clang::DeclRefExpr *, int> > &SortedParmRefs);

  void doAnalysis(void);

  bool hasUnmatchedParmArg(clang::ReturnStmt *RS, clang::CallExpr *CE);

  ReplaceCallExprVisitor *CollectionVisitor;

  llvm::DenseMap<clang::FunctionDecl *, ReturnStmtsVector *> FuncToReturnStmts;

  llvm::DenseMap<clang::ReturnStmt *, ParmRefsVector *> ReturnStmtToParmRefs;

  llvm::SmallVector<clang::CallExpr *, 10> AllCallExprs;

  clang::CallExpr *TheCallExpr;

  clang::ReturnStmt *TheReturnStmt;

  clang::FunctionDecl *CurrentFD;

  // Unimplemented
  ReplaceCallExpr(void);

  ReplaceCallExpr(const ReplaceCallExpr &);

  void operator=(const ReplaceCallExpr &);
};
#endif