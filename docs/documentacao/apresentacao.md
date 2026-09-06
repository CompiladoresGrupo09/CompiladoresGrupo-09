# 🚀 Interpretador de C — Grupo 09

## 1. A disciplina de Compiladores 1

Este projeto foi realizado por alunos da Universidade de Brasília durante a disciplina de Compiladores 1 (FGA0003), orientada pelo Prof. Dr. Sergio Antônio Andrade de Freitas.

A disciplina ensina a construção de um interpretador seguindo o pipeline léxico → sintático (com construção da AST) → semântico → interpretação recursiva da AST, usando Flex e Bison. Além do código, a proposta de trabalho exige que a equipe mantenha, ao longo de todo o desenvolvimento, um registro contínuo de decisões técnicas, planejamento de sprints e problemas/soluções — já que o professor só acessa o repositório na entrega final, e essa documentação é o que garante que o trabalho realizado seja visível e defensável na entrevista.

## 2. O projeto: Organizador de Compras no Mercado

Como estudo de caso para exercitar o núcleo da linguagem, a equipe escolheu um **organizador de compras de mercado**: o programa recebe itens e preços via `scanf` em loop, soma o total gasto e avisa se o valor ultrapassou o orçamento informado pelo usuário.

O interpretador cobre apenas o núcleo obrigatório da linguagem: tipos `int`/`float`/`char`, variáveis, expressões aritméticas/relacionais/lógicas com precedência, estruturas de controle (`if`/`else`, `while`, `for`), blocos com escopo, funções com parâmetros/retorno/recursão e entrada/saída básica (`printf`/`scanf`). Ponteiros, structs, pré-processador e otimizações ficam deliberadamente fora de escopo.

Ver [Definição da Linguagem](definicao-da-linguagem.md) para o exemplo completo de código e saída esperada.

## 3. Os Contribuidores

A equipe de desenvolvimento é composta por alunos da Universidade de Brasília.

<div class="team-container">

  <div class="team-card">
    <img src="docs/assets/Mariana_perfil.jpg" alt="Mariana Martins"/>
    <h3>Mariana Martins</h3>
    <div class="social-links">
      <a href="https://github.com/Marianamrts" target="_blank">GitHub</a>
    </div>
  </div>

  <div class="team-card">
    <img src="docs/assets/Eduardo_perfil.jpg" alt="Eduardo Ribeiro Xavier"/>
    <h3>Eduardo Ribeiro Xavier</h3>
    <div class="social-links">
      <a href="https://github.com/EduardoRibeiroXavier" target="_blank">GitHub</a>
    </div>
  </div>

  <div class="team-card">
    <img src="docs/assets/Giovana_perfil.jpg" alt="Giovana Ferreira"/>
    <h3>Giovana Ferreira</h3>
    <div class="social-links">
      <a href="https://github.com/gih7915" target="_blank">GitHub</a>
    </div>
  </div>

  <div class="team-card">
    <img src="docs/assets/Henrique_perfil.jpg" alt="Henrique Fontenelle"/>
    <h3>Henrique Fontenelle</h3>
    <div class="social-links">
      <a href="https://github.com/HenriqueFontenelle" target="_blank">GitHub</a>
    </div>
  </div>

  <div class="team-card">
    <img src="docs/assets/Brenda_perfil.jpg" alt="Brenda Beatriz"/>
    <h3>Brenda Beatriz</h3>
    <div class="social-links">
      <a href="https://github.com/Brwnds" target="_blank">GitHub</a>
    </div>
  </div>

</div>

<style>
.team-container {
  display: flex;
  flex-wrap: wrap;
  justify-content: center;
  gap: 25px;
  margin-top: 20px;
}

.team-card {
  width: 220px;
  background: linear-gradient(145deg, #f0f4f8, #d9e2ec);
  border-radius: 15px;
  padding: 20px;
  text-align: center;
  box-shadow: 0 6px 15px rgba(0,0,0,0.1);
  transition: transform 0.3s, box-shadow 0.3s;
  position: relative;
}

.team-card img {
  width: 120px;
  height: 120px;
  border-radius: 10px;
  object-fit: cover;
  margin-bottom: 15px;
}

.team-card h3 {
  font-size: 1.15em;
  color: #1f4e79;
  margin-bottom: 5px;
}

.team-card .social-links {
  display: flex;
  justify-content: center;
  gap: 15px;
  opacity: 0;
  transform: translateY(20px);
  transition: opacity 0.4s, transform 0.4s;
}

.team-card .social-links a {
  text-decoration: none;
  font-weight: bold;
  color: #1f4e79;
  transition: color 0.2s;
}

.team-card .social-links a:hover {
  color: #0d2b4f;
}

.team-card:hover {
  transform: translateY(-10px);
  box-shadow: 0 12px 25px rgba(0,0,0,0.2);
}

.team-card:hover .social-links {
  opacity: 1;
  transform: translateY(0);
}
</style>

## Histórico de Versão

| Data | Versão | Descrição | Autor |
|------|--------|-----------|-------|
| 05/09/2026 | 1.0 | Criação da página de apresentação e equipe | Brenda |
